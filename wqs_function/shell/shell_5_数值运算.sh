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

a=01
b=10
c=09
# 当数值是以0开头时，bash默认是8进制数，所以当遇到这种数时，需要显示的为其声明为十进制，在变量或数值前加上"10#"即可
((d=10#$a*60*60+10#$b*60+10#$c))
echo "d=$d"
