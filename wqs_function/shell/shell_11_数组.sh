# 1: 定义一个数组，定义这样的数组，它的下标是从0开始的
#arr_1=(1 2 3 4 5)
# 定义一个数组，也可以这样
arr_1[0]=1
arr_1[1]=2
arr_1[2]=3
arr_1[3]=4
arr_1[4]=5
echo "1: arr_1[0]=[${arr_1[0]}]"
echo "1: arr_1[3]=[${arr_1[3]}]"
# 通过for循环遍历数组
for arg in ${arr_1[@]}
do
    echo "1: arg=[$arg]"
done

# 2: shell脚本中数组中的成员除了是单一类型外，也可以是多类型的，下面将创建一个数字与字符串混合的数组
arr_2=(1 "second" 3 "fourth" 5)
echo "2: arr_2[1]=[${arr_2[1]}]"
echo "2: arr_2[2]=[${arr_2[2]}]"
# 通过for循环遍历数组
for arg in ${arr_2[@]}
do
    echo "2: arg=[$arg]"
done



# 3: 在shell脚本中，下标不一定是0-n这种数字，也可以是字符串，但是根据测试结果，当下标为字符串的时候，默认为下标0，所以实际上所有下标为字符串的操作，都是在下标0上进行操作的
arr_3["first"]="first"
arr_3["second"]="second"
echo "3: arr_3[\"first\"]=[${arr_3["first"]}]"
echo "3: arr_3=[${arr_3[@]}]"
for arg in ${arr_3[@]}
do
    echo "3: arg=[$arg]"
done

# 4: 获取数组的成员个数
echo "4: arr_1 length=[${#arr_1[@]}] or [${#arr_1[*]}]"

# 5: 获取某个下标的值
echo "5: arr_2[1]=[${arr_2[1]}]"

# 6: 对某个下标赋值，如果该下标存在，即修改该下标的值，如果该下标不存在，则新增该下标，并且该下标追加在数组的尾部
arr_1[0]=10
arr_1[6]=6
for arg in ${arr_1[@]}
do
    echo "6: arg=[$arg]"
done

# 7: 删除某个下标
unset arr_1[1]
for arg in ${arr_1[@]}
do
    echo "7: arg=[$arg]"
done

# 8: 清空整个数组
unset arr_1
for arg in ${arr_1[@]}
do
    echo "8: arg=[$arg]"
done

# 9: 访问数组的指定开始下标之后的元素，1表示开始的下标，4表示后续访问下标的个数
echo "9: arr_2=[${arr_2[@]}]"
for arg in ${arr_2[@]:1:4}
do
    echo "9: arg=[$arg]"
done

# 10: 模式替换，通过匹配的方式替换数组中元素的值，在百度时，有人说有这个功能，但是测试会报错，not found command
# ${arr_2[@]/1/100}
# echo "10: arr_2[0]=[${arr_2[0]}]"

# 11: 元素的值为空格时，可能会无法把该字符串看成一个整体
arr_11=(1 "hello world" 3 "fourth" 5)
echo "11: arr_11=[${arr_11[@]}]"
for arg in ${arr_11[@]}
do
    echo "11: arg=[$arg]"
done
# 所以需要特殊配置下，空格在脚本中会被认为是分隔符，通过设置IFS，可以将分隔符设置成其它的
IFS=$'\n'
echo "11: arr_11=[${arr_11[@]}]"
for arg in ${arr_11[@]}
do
    echo "11: arg=[$arg]"
done
