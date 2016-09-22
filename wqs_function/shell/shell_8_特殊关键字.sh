############################################################################ 1
{
	i=1
	while(($i))
	do
		echo "i=$i"
		((i=i-1))
	done
}&		#使语句块内的内容后台执行

wait 	#如果在wait语句前有转入后台执行的语句，则等待后台语句执行完毕

echo "wait End"

############################################################################ 2
echo "&&、|| and ()"
# command1 && command2 : 当command1执行成功时，执行command2
ls && echo "ls is success"
# command1 || command2 : 当command1执行失败时，执行command2
ls fdsa || echo "ls is failed"
# ( command1; command2; ......) 组合执行多个命令
(ls; ls $0 -l)

############################################################################ 3
echo "判断系统命令是否存在"
#command: 寻找内建和外部命令而非同名函数
command -v foo >/dev/null 2>&1 || { echo >&2 "I require foo but it's not installed. Aborting.";}
#type: 确定命令的源
type foo >/dev/null 2>&1 || { echo >&2 "I require foo but it's not installed. Aborting.";}
#hash: 记录并制定命令的路径名
hash foo 2>/dev/null || { echo >&2 "I require foo but it's not installed. Aborting.";}

############################################################################ 4
# 下面的格式，可以把filename和第二个END直接的内容写入到filename文件中
echo "cat << END > filename ..... END"

cat << END > tmp.txt
this is a simple file
second
third
END
