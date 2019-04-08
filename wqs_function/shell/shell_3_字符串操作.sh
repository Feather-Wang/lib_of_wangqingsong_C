#字符串操作

#字符串拼接 ###############################################
str1="wang"
str2="qing"
str3="str3 $str1$str2"
str4="str4 $str2"
str5="str5 ${str1}${str2}"
echo $str3
echo $str4
echo $str5

#字符串与命令返回结果拼接 #######################################
str="the time is $(date)"
echo $str

#如果字符串是一个系统命令，获取执行结果，要使用``将字符串括起来 #####################################
str20=`ls`
echo $str20

# 字符串包含判断
# 方法1
str1="wangqingsong"
str2="qing"
isHas=`echo $str1 | grep $str2`
if [ "$isHas" != "" ]
then
    echo "method 1 : str1 has str2"
fi
# 方法2
if [[ $str1 =~ $str2 ]]
then
    echo "method 2 : str1 has str2"
fi
# 方法3
case $str1 in
    *"$str2"*)
        echo "method 3 : str1 has str2"
        ;;
    *)
        echo "method 3 : str1 has not str2"
        ;;
esac
# 方法4
if [[ $str1 == *$str2* ]]
then
    echo "method 4 : str1 has str2"
fi


# {}的用法
# 用法1：get the len of the String ###################################
value1=${#str4}
echo $value1

# 用法2：get a child string from a big string #######################################
str10="alibaba is a great company"
value10=1
value11=4
# it will output "liba"，
# value10为截取的起始字节，计数从0开始，value11为截取的字节的个数，4表示截取4个字节
str11=${str10:$value10:$value11}
echo $str11

# 用法3：
# 说明:'#'表示拿掉左边的字符，'%'表示拿掉右边的字符
# 拿掉第一条'/'及其左边的字符
str="dir1/dir2/dir3/myfile.txt"
echo $str
str1=${str#*/}
echo "{} 3.1 --> str1 = $str1"
# 拿掉最后一个'/'及其左边的字符
str2=${str##*/}
echo "{} 3.2 --> str2 = $str2"
# 拿掉指定的第几个的'/'及其左边的字符，不能指定第一个，只能从第二个开始算，第二个'/'的序列号是1，第三个是2，以此类推
str3=${str#*1/}
echo "{} 3.3 --> str3 = $str3"
# 拿掉第一条'.'及其左边的字符，其他用于与拿到'/'一样，只需要将'/'换成想要指定的字符就行
str4=${str#*.}
echo "{} 3.4 --> str4 = $str4"
# 拿掉右数第一个'/'及其右边的字符
str5=${str%/*}
echo "{} 3.5 --> str5 = $str5"
# 拿掉右数最后一个'/'及其右边的字符
str6=${str%%/*}
echo "{} 3.6 --> str6 = $str6"
echo ""

# 用法4 ：字符串替换
str="dir1/dir2/dir3/myfile.txt"
# 使用"path"替换掉第一个"dir"
str1=${str/dir/path}
echo "{} 4.1 --> str1 = $str1"
# 使用"path"替换掉全部的"dir"
str2=${str//dir/path}
echo "{} 4.2 --> str2 = $str2"

# 用法5：根据变量不同的状态赋值
str=""
# 当str为空值，使用"myfile.txt"为默认值，(保留没设定及非空值)
str1=${str-myfile.txt}
echo "{} 5.1 --> str = $str, str1 = $str1"
# 当str为空值或没有设定，使用"myfile.txt"为默认值，(保留非空值)
str2=${str:-myfile.txt}
echo "{} 5.2 --> str = $str, str2 = $str2"
# 不管str为何值，均使用"myfile.txt"为默认值，(不保留任何值)
str3=${str+myfile.txt}
echo "{} 5.3 --> str = $str, str3 = $str3"
# 除非str为空值，否则使用"myfile.txt"为默认值，(保留空值)
str4=${str:+myfile.txt}
echo "{} 5.4 --> str = $str, str4 = $str3"
# 当str没设定时，使用"myfile.txt"为默认值，(保留空值及非空值)
str5=${str=myfile.txt}
echo "{} 5.5 --> str = $str, str5 = $str5"
# 当str没设定或空值，使用"myfile.txt"为默认值，同时str设置为"myfile.txt"，(保留非空值)
str6=${str:=myfile.txt}
echo "{} 5.6 --> str = $str, str6 = $str6"
# 当str没设定时，将"myfile.txt"输出至STDERR，(保留空值及非空值)
str7=${str?myfile.txt}
echo "{} 5.7 --> str = $str, str7 = $str7"
# 当str没设定或空值时，将"myfile.txt"输出至STDERR，(保留非空值)
str8=${str?myfile.txt}
echo "{} 5.8 --> str = $str, str8 = $str8"

# 用法6：字符串转换数字。10表示10进制，变量不用{}括起来也可以
StartHour=09
StartMinute=09
StartTime=$((10#${StartHour} * 60 + 10#$StartMinute))
