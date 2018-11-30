
echo -n "Input var : "

# 通过手动输入获取信息
read var1 var2 var3  var4 

echo "var1 : $var1"
echo "var2 : $var2"
echo "var3 : $var3"
echo "var4 : $var4"

# 通过管道获取信息，这种方式从管道中读取信息时，是阻塞式的
# 向管道中写数据时，可以通过任何正常的输出方式，然后通过'>'和'>>'将内容重定向到管道文件
if [ ! -p pipe ]
then
    #首先创建管道
    mknod pipe p
fi
read var5 < pipe
echo "var5=[$var5]"

# 通过管道循环读取信息，这种方式从管道中读取信息时，是非阻塞式的，所以为了打印无关信息，要对读取的信息做判断，如果是空，就continue
while [ 1 ]
do
    read msg
    if [ "$msg" = "" ]
    then
        continue
    else
        echo "msg=[$msg]"
    fi
done < pipe
