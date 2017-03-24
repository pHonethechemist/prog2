i="1020"

while [ $i -lt 1030 ]; do
  ./strassen 0 $i $i.txt &
  i=$[$i+1]
done
