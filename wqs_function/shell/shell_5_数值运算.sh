echo -n "Input two data : "

read data1 data2 

((data = data1 - data2))
#data=`expr $data1 - $data2`
echo "$data1 - $data2 = $data"

((data = data1 + data2))
#data=`expr $data1 + $data2`
echo "$data1 + $data2 = $data"

((data = data1 * data2))
#data=`expr $data1 \* $data2`
echo "$data1 * $data2 = $data"

data=`expr $data1 / $data2`
echo "$data1 / $data2 = $data"

data=`expr $data1 % $data2`
echo "$data1 % $data2 = $data"
