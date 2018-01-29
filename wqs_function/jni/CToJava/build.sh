# 用于直接编译光大银行获取节点状态出口的代码.
# 使用步骤：
# 1、放在CToJava目录下，使用命令“. ./build.sh"直接编译。
# 2、运行./a.out
#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JAVA_HOME/jre/lib/amd64/server
export CLASSPATH=$CLASSPATH:.

CC=cc 

echo "compile Java ..."
javac Demo.java
echo "compile C ..."
cc jni_04_self-defining-class.c -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -L$JAVA_HOME/jre/lib/amd64/server -ljvm
