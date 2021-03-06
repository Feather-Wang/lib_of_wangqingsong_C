#!/bin/bash

# 本工具依赖以下系统命令：ls、date、pwd、cat、wc、expr、rm、sed、echo、cut、getopt、grep

CONFIG_FILE="$WQS_PATH/wqs_tools/wqs_rm/wqs_rm.conf"
HISTORY_FILE="$WQS_PATH/wqs_tools/wqs_rm/history_file"
TRASH_DIR=""
TRASH_PATH=""


if [ -f $CONFIG_FILE -a -e $CONFIG_FILE ]
then
    TRASH_DIR=`cat $CONFIG_FILE | awk -v home=$HOME 'BEGIN{FS="=";}
    {
        if( $1 == "TRASH_DIR" ){
            array["TRASH_DIR"] = $2;
        }
    }
    END{
    if( "TRASH_DIR" in array ){
        print array["TRASH_DIR"]"/trash_wqs";
    }else{
    print home"/trash_wqs";
}
}'`
else
    touch $CONFIG_FILE
    TRASH_DIR="$HOME/trash_wqs"
fi

if [ -e $HISTORY_FILE ]
then
    HISTORY_NUMBER=`cat $HISTORY_FILE | wc -l`
else
    HISTORY_NUMBER="0"
fi
(( HISTORY_NUMBER = HISTORY_NUMBER + 1 ))
HISTORY_NUMBER_LEN=${#HISTORY_NUMBER}
if [ $HISTORY_NUMBER_LEN -lt 2 ]
then
    HISTORY_NUMBER="000$HISTORY_NUMBER"
elif [ $HISTORY_NUMBER_LEN -lt 3 ]
then
    HISTORY_NUMBER="00$HISTORY_NUMBER"
else
    HISTORY_NUMBER="0$HISTORY_NUMBER"
fi

DATE=`date +%Y_%m_%d`
TIME=`date +%H:%M:%S`
path_command=`pwd`

function Usage()
{
    echo "--help		            show all arguments"
    echo "--clean		        delete trash and history"
    echo "--history              show command history"
    #echo "--history-clean        delete the history"
    echo "-r|--recovery=num      recovery file, num is the number in history, e.g. del -r1 or del --recovery=1"
    echo "--trashpath=<path>                  set the trash directory, the init path is \$HOME/trash_wqs"
    echo "--view-profile        view profile"
    echo "--size                view trash path size"
    echo "--rm                  \"rm\" command use to del file"
}

# 清理$TRASH_DIR目录，保留5天内的删除文件
function Clean()
{
    all_day=`ls $TRASH_DIR`
    num=`ls $TRASH_DIR | wc -l`
    #echo $all_day":"$num

    for day in $all_day
    do
        #if [ $num -le 5 ]
        #then
        #    return 0
        #fi

        #echo "num="$num" day = "$day
        rm -rf $TRASH_DIR/$day
        num=`expr $num - 1`
        #echo "num="$num
    done
}

# 清空历史记录
function History_CLEAN()
{
    rm -f $HISTORY_FILE
    touch $HISTORY_FILE
}

# 显示删除历史记录
function History_SHOW()
{
    cat $HISTORY_FILE
}

# 恢复删除文件，具有以下功能
# 1、删除文件和当前目录下是否存在要恢复的文件
# 2、根据历史文件记录进行文件恢复处理
function Recovery()
{
    file_trash=""
    file_old=""

    name_old=""
    time_trash=""
    path_trash=""
    path_old=""

    # 获取指定行号的信息
    line=`sed -n "$@p" $HISTORY_FILE`
    if [ "$line" == "" ]
    then
        echo "there is not $@ history"
        return 1
    fi

    # 从信息中提取指定的信息
    str=`echo "$line" | cut -d ' ' -f 2`
    path_trash="$TRASH_DIR/$str"
    str=`echo "$line" | cut -d ' ' -f 3`
    time_trash="$str"
    num_line=`echo "$line" | awk '{print NF}'`
    str=`echo "$line" | cut -d ' ' -f $num_line`
    path_old="$str"

    # 循环读取文件，因为一条记录可能包含多个文件
    for((i=5;i<$num_line;i++))
    do
        name_old=`echo "$line" | cut -d ' ' -f $i`
        name_trash="${name_old##*/}_${time_trash}"

        # 判断文件信息是否为绝对路径，如果不是绝对路径的话，要把文件信息和目录信息组成一个绝对路径
        isRootPath=${name_old:0:1}
        if [[ "/" = "$isRootPath" ]]
        then
            file_old="$name_old"
        else
            file_old="$path_old/$name_old"
        fi
        file_trash="$path_trash/$name_trash"

        #echo "file_trash=[$file_trash]"
        #echo "file_old=[$file_old]"

        # 判断源目录下是否存在同名文件
        if [ -e $file_old ]
        then
            echo "$file_old is exist"
            return 2
        fi

        # 判断回收站中是否存在备份文件
        if [ ! -e $file_trash ]
        then
            echo "$file_trash is already deleted"
            return 3
        fi

        # 进行文件还原
        mv $file_trash $file_old
    done
}

function SetTrashPath()
{
    isHave=`cat $CONFIG_FILE | grep "TRASH_DIR"`
    if [ "" != "$isHave" ]
    then
        sed -i '/TRASH_DIR/d' $CONFIG_FILE
        echo "TRASH_DIR=$1" >> $CONFIG_FILE
    else
        echo "TRASH_DIR=$1" >> $CONFIG_FILE
    fi
}

function ViewProfile()
{
    cat $CONFIG_FILE
}

function ShowSize()
{
    du -s $TRASH_DIR -h
    df -h
}

function RealRm()
{
    rm -r $1
}

# 处理命令参数，然后调用相应的处理函数
function opt_process()
{
    #echo "opt_process --> " $@
    args=`getopt -u -o "hr:" --long "help,clean,history,history-clean,recovery:,trashpath:,view-profile,size,rm:" -- "$@"`
    set -- ${args}
    while [ -n "$1" ]
    do
        case $1 in
            -h|--help)
            Usage
            shift
            ;;
            --clean)
            Clean
            History_CLEAN
            shift
            ;;
            --history)
            History_SHOW
            shift
            ;;
            #-d|--history-clean)
            #    History_CLEAN
            #    shift
            #    ;;
            -r|--recovery)
            Recovery $2
            shift
            ;;
            --trashpath)
            SetTrashPath $2
            shift
            ;;
            --view-profile)
            ViewProfile
            shift
            ;;
            --size)
            ShowSize
            shift
            ;;
            --rm)
            RealRm $2
            shift
            ;;
            --)
            shift
            ;;
            *)
            Usage
            shift
            ;;
        esac
        shift
    done


    exit
}

############################################### 命令行参数处理
############### 进行参数判断，脚本不能同时处理命令参数和文件处理
isCommand=0
isFile=0
for arg in $@
do
    arg_one=${arg:0:1}
    arg_two=${arg:0:2}
    #echo "arg_one=[$arg_one], arg_two=[$arg_two]"
    if [[ "-" = "$arg_one" || "--" = "$arg_two" ]]
    then
        isCommand=1
    else
        isFile=1
    fi
done

if [ $isCommand -eq 1 -a $isFile -ne 1 ] # 只有命令参数
then
    opt_process $@
elif [ $isCommand -eq 1 -a $isFile -eq 1 ] # 同时存在命令参数和文件
then
    echo "this command can't run with command and file"
    exit
    #else
    # echo "Start Delete File ..."
fi

##############################################################
# 判断回收站目录是否存在，不存在则创建
if [ ! -e $TRASH_DIR ]
then
    mkdir $TRASH_DIR
fi

TRASH_PATH="$TRASH_DIR/$DATE"

# 判断回收站目录下是否存在当前日期的目录，不存在则创建
Judge=`ls $TRASH_DIR | grep "^$DATE$"`
if [ "$Judge" = "" ]
then
    mkdir $TRASH_DIR/$DATE
fi
#echo $TRASH_PATH

FILELIST[0]=""
index_filelist=0
# 进行文件处理，利用mv命令，将要删除的文件移动到回收站目录下
for arg in "$@"
do
    len=${#arg}
    #echo $len

    if [ "$len" -eq "0" ]
    then
        echo "nothing input"
        exit 0
    elif [ "$arg" == "/" ]
    then
        echo "Do you want to rm the root path ?"
        exit 0
    fi

    # 获取最后一个字符，判断是否为'/'，如果为'/'，则去掉最后得'/'
    # 这是因为当删除得是目录时，可能最后会带一个'/'，从而导致移动失败
    str=${arg:$len-1:1}
    if [ "$str" = "/" ]
    then
        arg=${arg:0:$len-1}
    fi

    # 判断要处理得文件是不是绝对路径，如果为不是的话，就跟当前路径组成一个绝对路径
    isRootPath=${arg:0:1}
    if [[ "/" != "$isRootPath" ]]
    then
        arg=$path_command/$arg
    fi

    # 获取文件名，和当前时间组成一个新的文件名
    newFile=${arg##*/}
    newFile="${TRASH_PATH}/${newFile}_$TIME"

    # 将当前处理的文件信息添加到数组FILELIST中，用于处理完后填写历史记录
    FILELIST[$index_filelist]=$arg
    index_filelist=`expr $index_filelist + 1`

    #echo "arg=$arg"
    #echo "newFile=$newFile"

    # 备份文件或目录
    mv $arg $newFile
done

# 将本次删除记录到历史文件中
echo -e "$HISTORY_NUMBER $DATE $TIME del ${FILELIST[@]} $path_command" >> $HISTORY_FILE

# echo "Finished ..."

exit 0
