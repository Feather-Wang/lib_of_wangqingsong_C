# 函数定义必须在调用之前

# 关于返回值，使用return返回数值，且只能返回数字，数字取值：0-255,
# 如果没有指定，返回上次状态码，不能返回变量，返回变量就会是255，只能返回常量值
# 调用 : test_function A B ASD，像使用Linux命令一样，直接使用函数名就行，也可以在``中使用
# 传参 : 与运行shell脚本一样，参数依次跟在函数名后面，在函数中获取参数与shell一样，使用$0-n，$0为shell脚本名，$1为第一次参数，依次类推

# 关于调用其他shell中的函数，在调用前，执行". a.sh"，点+脚本名，然后就可以使用该脚本中的函数了，注：如果在两个脚本中有重名的函数，执行的最后一个引用的脚本中的函数

# 定义函数的两种方式
# 方式一
test_function()
{
	echo "1 parm : $1"
	echo "2 parm : $2"
	echo "3 parm : $3"
	echo "test_function"

	return 125
}

# 方式二
function test_function1()
{
	echo "test_function1"
}

# 调用函数
echo "function start"
test_function A B ASD
echo "test_function return value : [ $? ]"
string=`test_function1`
echo "string : $string"
