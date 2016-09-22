#文件判断

#文件是目录文件
if [ -d $0 ]
then
	echo "$0 is a path"
fi

#文件是一般文件
if [ -f $0 ]
then
	echo "$0 is a file"
fi

#文件存在
if [ -e $0 ]
then
	echo "$0 is exist"
fi

#文件不是空的
if [ -s $0 ]
then
	echo "$0 has data"
fi

#文件权限 可读
if [ -r $0 ]
then
	echo "$0 can be read"
fi

#文件权限 可写
if [ -w $0 ]
then
	echo "$0 can be write"
fi

#文件权限 可执行或者如果是目录的话，可以存放文件
if [ -x $0 ]
then
	echo "$0 can be run , or $0 is a path and can has some file"
fi

#字符串判断
str1=""
str2="wang"
if [ -n $str2 ]
then
	echo "str2 is not null"
fi

if [ -z $str1 ]
then
	echo "str1 is null"
fi
# 字符串比较
if [ "$str1" = "$str2" ]
then
    echo "str1 == str2"
fi

#数值判断
value=100
if [ $value -eq 100 ]
then
	echo "$value == 100"
fi

if [ $value -ne 101 ]
then
	echo "$value != 101"
fi

if [ $value -lt 101 ]
then
	echo "$value < 101"
fi

if [ $value -gt 99 ]
then
	echo "$value > 99"
fi

if [ $value -le 100 ]
then
	echo "$value <= 100"
fi

if [ $value -ge 100 ]
then
	echo "$value >= 100"
fi
