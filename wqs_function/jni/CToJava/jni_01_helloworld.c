#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int jni_create(JavaVM **jvm, JNIEnv **env)
{
    int ret = -1;

    JavaVMInitArgs vm_args;
    JavaVMOption options[9];

    /*屏蔽即时编译*/
    options[0].optionString = "-Djava.compiler=NONE";
    /*设置CLASSPATH*/
    options[1].optionString = "-Djava.class.path=.:/home/wangqingsong/test/jdk1.7/jdk1.7.0_79/lib/tools.jar:/home/wangqingsong/test/jdk1.7/jdk1.7.0_79/lib/dt.jar:";
    options[2].optionString = "-Djava.library.path=.:/home/wangqingsong/test/jdk1.7/jdk1.7.0_79/lib/tools.jar:/home/wangqingsong/test/jdk1.7/jdk1.7.0_79/lib/dt.jar:";
    //options[2].optionString = "-verbose:NONE";
    /*设置JVM内存初始值*/
    options[3].optionString = "-Xms4M";
    /*设置JVM内存最大值*/
    options[4].optionString = "-Xmx64M";
    /*本地堆栈大小*/
    options[5].optionString = "-Xss512K";
    /*Java堆栈大小*/
    options[6].optionString = "-Xoss400K";

    /*设置VM参数*/
    vm_args.version = JNI_VERSION_1_6;   
    /*options设置了多少个参数*/
    vm_args.nOptions = 7;   
    vm_args.options = options;

    /*JNI_FALSE: JNI_CreateJavaVM return JNI_ERR as soon as it encounters any unrecognized option strings*/
    vm_args.ignoreUnrecognized = JNI_FALSE;

    ret = JNI_CreateJavaVM(jvm, (void**)env, &vm_args);   
    if (ret < 0)   
    {   
        puts("JNI_CreateJavaVM failed");
        return -1;
    }  

    return 0;
}

/*hello world*/
int test_01()
{
    int ret = -1;
    
    JavaVM* jvm;
    JNIEnv *env = NULL;

    /*创建java虚拟机*/
    ret = jni_create(&jvm, &env);
    if( -1 == ret )
    {
        puts("jni_create failed");
        return -1;
    }

    /*找到指定类*/
    jclass cls = (*env)->FindClass(env, "Demo");
    puts("FindClass");

    /*获取构造函数ID*/
    jmethodID mid = (*env)->GetMethodID(env, cls, "<init>","()V");  
    puts("GetMethodID init");

    /*新建对象*/
    jobject obj = (*env)->NewObject(env, cls, mid);
    puts("NewObject");

    /*映射类中的方法*/
    jmethodID func = (*env)->GetMethodID(env, cls, "HelloWorld", "()V");

    /*调用方法*/
    (*env)->CallVoidMethod(env, obj, func);   

    /*关闭JVM*/
    (*jvm)->DestroyJavaVM(jvm);   
    
    return 0;   
}

int main(void)
{
    test_01();

    return 0;
}
