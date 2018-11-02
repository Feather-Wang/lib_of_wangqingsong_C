WQS_PATH="$HOME/lib_of_wangqingsong_C"
WQS_NEWPATH="$PATH"

#添加环境变量：代码目录
export CODE=${WQS_PATH}/wqs_function
export WQS_PATH=$WQS_PATH

# 添加自定义命令
. $HOME/lib_of_wangqingsong_C/tool.sh


# 添加库目录环境变量
. $HOME/lib_of_wangqingsong_C/lib.sh


#更新PATH环境变量
if [ "$WQS_NEWPATH" != "$PATH" ]
then
	export PATH=$WQS_NEWPATH
fi

# 更新当前系统时间，需要联网，针对虚拟机做的，虚拟机使用休眠功能时，系统时间会与真实时间脱节
screen_count=`who | awk 'BEGIN{i=0} {len=length($5)} len>7{i=i+1} END{print i}'`
if [ $screen_count -le 1 ]
then
    sudo /usr/sbin/ntpdate -u ntp.api.bz
fi
