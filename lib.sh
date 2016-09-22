#添加环境变量：安装第三方库目录
wqs_LIB=${wqs_PATH}/wqs_lib
export LIB=${wqs_LIB}

#添加环境变量：安装第三方库头文件目录
JudgeLIB=`echo $C_INCLUDE_PATH | grep $wqs_LIB`
if [ -z $JudgeLIB ]
then
	export C_INCLUDE_PATH=${wqs_LIB}/include:$C_INCLUDE_PATH
fi

#添加环境变量：安装第三方库动态库目录
JudgeLIB=`echo $LD_LIBRARY_PATH | grep $wqs_LIB`
if [ -z $JudgeLIB ]
then
	export LD_LIBRARY_PATH=${wqs_LIB}/lib:$LD_LIBRARY_PATH
fi
#添加环境变量：安装第三方库静态库目录
JudgeLIB=`echo $LIBRARY_PATH | grep $wqs_LIB`
if [ -z $JudgeLIB ]
then
	export LIBRARY_PATH=${wqs_LIB}/lib:$LIBRARY_PATH
fi

wqs_NEWPATH="${wqs_LIB}/bin:$wqs_NEWPATH"
