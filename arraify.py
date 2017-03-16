import sys

f = open(sys.argv[1], 'r')
d = int(sys.argv[2])

ret = ''
c = 0
r = 0
for line in f:
    if c == 0:
        ret += '{'
        if r == 0:
            ret += '{'
    if c == d - 1:
        c = 0
        ret += (line.replace('\n', '') + '},\n')
        if r == d - 1:
            r = 0
            ret = ret[:-2] + '}\n'
        else:
            r += 1
    else:
        c += 1
        ret += (line.replace('\n', '') + ',')

f.close()
o = open('mat.txt', 'w')
o.write(ret)
o.close()
