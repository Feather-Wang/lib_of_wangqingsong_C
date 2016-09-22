wqs_PATH="$HOME/lib_of_wangqingsong_C"
wqs_NEWPATH="$PATH"

Config_PATH="$HOME/.bashrc"

#添加环境变量：代码目录
wqs_CODE=${wqs_PATH}/wqs_function
export CODE=${wqs_CODE}

# 添加自定义命令
. $HOME/lib_of_wangqingsong_C/tool.sh


# 添加库目录环境变量
. $HOME/lib_of_wangqingsong_C/lib.sh


#更新PATH环境变量
if [ "$wqs_NEWPATH" != "$PATH" ]
then
	export PATH=$wqs_NEWPATH
fi
