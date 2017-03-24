import os
import sys
import re
import matplotlib.pyplot as plt

line_pattern = r'(?P<type>normalmultiply |n0 = )(?P<n0>[0-9]+): (?P<secs>[0-9]+)s(?P<msecs>[0-9]+)ms(?P<usecs>[0-9]+)us'
lp = re.compile(line_pattern)

file_pattern = r'([0-9]+)res[0-9]*\.txt'
fp = re.compile(file_pattern)


root = sys.argv[1]
ns = []
n0s = {}
ts = {}
files = []

if len(sys.argv) > 2:
	files = sys.argv[2:]
else:
	files = os.listdir(root)

for file in files:
	f = open(os.path.join(root, file), 'r')
	rf = fp.search(file)
	n = int(rf.group(1))
	if n not in list(n0s.keys()):
		n0s[n] = []
		ts[n] = []
	for line in f:
		l = lp.search(line)
		if l.group('type') == "n0= ":
			n0 = int(l.group('n0'))
			n0s[n].append(n0)
		else:
			n0s[n].append(n)
		t = int(l.group('secs')) + .001 * int(l.group('msecs')) + .0000001 * int(l.group('usecs'))
		ts[n].append(t)
	# plt.figure()
	# plt.xlim(0, max(n0s))
	# plt.ylim(0, 20)
	# plt.title("Dimension " + str(n))
	# plt.xlabel("n0")
	# plt.ylabel("Time (s)")
	# plt.plot(n0s, ts, 'bo')
	# n0s = []
	# ts = []
	f.close()

ns = sorted(n0s.keys())

for n in ns:
	plt.plot(n0s[n], ts[n], marker = 'o', linestyle = "None", alpha=0.7, label= "n=" + str(n))

plt.title("Standard Matrix Multiplication")
plt.xlabel("n")
plt.ylabel("Time (s)")
# plt.xlim(20, 70)
# plt.legend(numpoints=1)
plt.show()
plt.close()