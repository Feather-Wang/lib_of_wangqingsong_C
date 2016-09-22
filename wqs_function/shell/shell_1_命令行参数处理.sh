# 该脚本，主要演示了shell脚本的命令行参数的获取、处理，以及一些特殊的全局变量

#状态值，成功(0)，失败(1)
echo "status is [ $? ]"

#当前shell的PID
echo "this shell's PID is [ $$ ]"
#shell最后运行的后台的PID
echo "this shell's PID is [ $! ]"

#参数的个数，不包括命令本身
echo "the count of argument is $#"

# $*和$@类似，都会返回所有的参数，不同的是前者是一个字符串，而后者是一个数组，配合for循环使用，可以使用$@单独读取一个参数，$@必须加引号
echo "all of the argument 1 is [ $* ]"
echo "all of the argument 2 is [ $@ ]"

echo "PID number is $!"

# $0,$1....获取指定的参数，其中$0是脚本名称，$1是第一个参数，$2是第二个参数，$9是第九个参数，10以上的取参，数字必须用{}括起来
echo "the first argument is [ $0 ]"
echo "the second argument is [ $1 ]"
echo "the third argument is [ $2 ]"
echo "the fourth argument is [ $3 ]"
echo "the fifth argument is [ $4 ]"
echo "the sixth argument is [ $5 ]"
echo "the seventh argument is [ $6 ]"
echo "the eighth argument is [ $7 ]"
echo "the ninth argument is [ $8 ]"
echo "the tenth argument is [ $9 ]"
echo "the eleventh argument is [ ${10} ]"
echo "the twelveth argument is [ ${11} ]"

#使用set命令设定的Flag一览
echo "传递到脚本中的标志是 [ $- ]"
num="wangqingsong"

echo "之前命令的最后一个参数是 [ $_ ]"

#处理参数的方法有三种
#第一种，挨个获取参数，然后处理，该方法严重依赖参数的位置，通常用于处理建立的参数
#第二种，使用getopts，该工具由Bash内置了，能支持短选项，比如-o a.out这种，但不支持长选项，比如--prefix=/home这种
#第三种，使用getopt，该工具是独立的可执行文件，能够支持短选项和长选项

#getopts实例，其中a的后面有冒号，表示该选项需要参数
echo "getopts ...................."
while getopts "a:b:c" arg
do
	case $arg in
		a)
			echo "this is a, the argument is [ $OPTARG ]"
			;;
		b)
			echo "this is b, the argument is [ $OPTARG ]"
			;;
		c)
			echo "this is c"
			;;
		?)
			echo "unkown argument"
	esac
done


echo "getopt ...................."
args=`getopt -u -o "a:b:c" -l "about:,bit:,ctrl" -- "$@"`
echo "args = [ $args ]"
set -- ${args}

while [ -n "$1" ]
do
	case $1 in
		-a|--about)
			echo "this is -a|--about, argument is $2"
			shift
			;;
		-b)
			echo "this is -b|--bit, argument is $2"
			shift
			;;
		-c)
			echo "this is -c"
			;;
		--)
			echo "-- is [ $1 ]"
			shift
			break
			;;
		*)
			echo "$1 is not option"
			;;
	esac
	shift
done
