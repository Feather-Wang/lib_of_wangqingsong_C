# 演示内容：控制语句

#for循环
echo "Show For ----------------------------"
str=`ls`
for arg in $str
do
	echo $arg
done

for((i = 0; i <= 200; i++))
do
	echo "i = $i"
done


# until和while的不同之处在于，while:条件为真，则继续执行，until:条件为假，则继续执行
#while循环
echo "Show While ----------------------------"
str1="wang"
while [ "$str1" != "" ]
do
	echo $str1
	str1=${str1:0,1}
done
i=10
while(($i))
do
	echo "i = $i"
	((i=i-1))
done



# until循环
echo "Show until ----------------------------"
str1="qing"
until [ "$str1" = "" ]
do
	echo $str1
	str1=""
done

#if语句
echo "Show if ----------------------------"
str2="/"
if [ "$str2" = "/" ]
then
	echo "Yes"
elif [ "$str2" = "\\" ]
then
    echo "Yes 2"
else
	echo "No"
fi

if [ "$str2" = "/" ]
then
	echo "Yes"
fi

echo "Show case ----------------------------"
#switch语句
#value="d"
value=$1
case $value in
	a)
		echo "this is a"
		;;
	b|c)
		echo "this is b and c"
		;;
	?)
		echo "i don't know this value $value"
		;;
	*)
		echo "*******************************"
		;;
esac
