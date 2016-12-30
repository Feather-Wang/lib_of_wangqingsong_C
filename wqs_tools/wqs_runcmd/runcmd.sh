#!/bin/bash
# 注意
# 在执行本脚本时，设置的参数必须放在执行的命令的前面
# 如果需要使用“|”，请把“|”使用“！”替代，在脚本中会将“！”替换成“|”

function Usage()
{
    echo "please put option before the command"
    echo "if you want to use '|', please use '!'"
    echo "-c|--count run times"
    echo "-s|--sleeptime the seconds ot running"
}

var_count=1
var_sleeptime=1
g_cmd=""
g_option=""

# 获取要执行的命令
get_cmd=0
optnum=0
for arg in $@
do
    is_cmd=${arg:0:1}
    # 获取设置的本脚本的参数，放在g_option数组中，供后面的getopt命令使用
    if [[ $get_cmd -eq 0 && "-" =~ $is_cmd ]]
    then
        g_option[$optnum]=$arg
        (( optnum = optnum + 1 ))
    else
        # 当检测到第一个没有以‘-’开头的参数时，默认为后面的都属于要执行的命令部分
        get_cmd=1
    fi

    # 当设置了get_cmd后，将后面的参数都放到执行命令的变量g_cmd中
    if [ $get_cmd -eq 1 ]
    then
        if [[ $is_cmd =~ "!" ]]
        then
            g_cmd="$g_cmd |"
        else
            g_cmd="$g_cmd $arg"
        fi
    fi
done
#echo "g_cmd = $g_cmd"
#echo "g_option is ${g_option[@]}"

# 解析g_option命令参数，设置相关变量
args=`getopt -u -o "c:s:h" -l "count:,sleeptime:,help" -- "${g_option[@]}"`
set -- ${args}
while [ -n "$1" ]
do
    case $1 in
        -h|--help)
            Usage
            shift
            ;;
        -c|--count)
            #echo "this is -c|--count, argument is $2"
            var_count=$2
            shift
            ;;
        -s|--sleeptime)
            #echo "this is -s|--sleeptime, argument is $2"
            var_sleeptime=$2
            shift
            ;;
        --)
            #echo "-- is [ $1 ]"
            shift
            ;;
        *)
            echo "$1 is not option"
            ;;
    esac
    shift
done

while(($var_count))
do
    eval $g_cmd
    sleep $var_sleeptime
    if [ $var_count -gt 0 ]
    then
        ((var_count = var_count - 1))
    fi
done
