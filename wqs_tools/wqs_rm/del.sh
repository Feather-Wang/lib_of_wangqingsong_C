TRASH_DIR="$HOME/trash_wqs"
TRASH_PATH=""
HISTORY_FILE="$HOME/lib_of_wangqingsong_C/wqs_tools/wqs_rm/history_file"
HISTORY_NUMBER=`cat $HISTORY_FILE | wc -l`
(( HISTORY_NUMBER = HISTORY_NUMBER + 1 ))

DATE=`date +%Y_%m_%d`
TIME=`date +%H:%M:%S`

function Usage()
{
    echo "--help		    show all arguments"
    echo "--clean		    delete trash except near 5 days"
    echo "--history         show command history"
    echo "--history-clean   delete the history"
    echo "--recovery=num    num is the number in history"
}

function Clean()
{
    all_day=`ls $TRASH_DIR`
    num=`ls $TRASH_DIR | wc -l`
    #echo $all_day":"$num

    for day in $all_day
    do
        if [ $num -le 5 ]
        then
            return 0
        fi

        #echo "num="$num" day = "$day
        rm -rf $TRASH_DIR/$day
        num=`expr $num - 1`
        #echo "num="$num
    done
}
function Recovery()
{
    file_trash=""
    file_old=""

    name_old=""
    name_trash=""
    path_trash=""
    path_old=""

    line=`sed -n "$@p" $HISTORY_FILE`
    if [ "$line" == "" ]
    then
        echo "there is not $@ history"
        exit
    fi
    
    for((i=1;i<=6;i++))
    do
        str=`echo "$line" | cut -d ' ' -f $i`
        if [ $i -eq 2 ]
        then
            path_trash="$HOME/trash_wqs/$str"
        elif [ $i -eq 3 ]
        then
            name_trash="$str"
        elif [ $i -eq 5 ]
        then
            name_old="$str"
            name_trash="${str}_${name_trash}"
        elif [ $i -eq 6 ]
        then
            path_old="$str"
        fi
    done

    file_old="$path_old/$name_old"
    file_trash="$path_trash/$name_trash"

    if [ -e $file_old ]
    then
        echo "$file_old is exist"
        exit
    fi

    if [ ! -e $file_trash ]
    then
        echo "$file_trash is already deleted"
        exit
    fi

    mv $file_trash $file_old
}

function History_SHOW()
{
    cat $HISTORY_FILE
}
function History_CLEAN()
{
    rm -f $HISTORY_FILE
    touch $HISTORY_FILE
}

function opt_process()
{
    #echo "opt_process --> " $@
    args=`getopt -u -o "" -l "help,clean,history,history-clean,recovery:" -- "$@"`
    set -- ${args}
    ret=0
    while [ -n "$1" ]
    do
        case $1 in
            --help)
                Usage
                ret=1
                shift
                ;;
            --clean)
                Clean
                ret=1
                shift
                ;;
            --history)
                History_SHOW
                ret=1
                shift
                ;;
            --history-clean)
                History_CLEAN
                ret=1
                shift
                ;;
            --recovery)
                Recovery $2
                ret=1
                shift
                ;;
            *)
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
    arg_str=${arg:0:2}
    if [[ $arg_str =~ "--" ]]
    then
        isCommand=1
    else
        isFile=1
    fi
done

if [ $isCommand -eq 1 -a $isFile -ne 1 ]
then
    opt_process $@
elif [ $isCommand -eq 1 -a $isFile -eq 1 ]
then
    echo "this command can't run with command and file"
    exit
fi
##############################################################
if [ ! -e $TRASH_DIR ]
then
    mkdir $TRASH_DIR
fi

TRASH_PATH="$TRASH_DIR/$DATE"

Judge=`ls $TRASH_DIR | grep "^$DATE$"`
if [ "$Judge" = "" ]
then
    mkdir $TRASH_DIR/$DATE
fi
#echo $TRASH_PATH

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

    str=${arg:$len-1:1}

    if [ "$str" = "/" ]
    then
        arg=${arg:0:$len-1}
    fi
    newFile=${arg##*/}
    newFile="${TRASH_PATH}/${newFile}_$TIME"

    #echo "arg=$arg"
    #echo "newFile=$newFile"

    mv $arg $newFile
done

path_command=`pwd`
echo -e "$HISTORY_NUMBER $DATE $TIME del $@ $path_command" >> $HISTORY_FILE
