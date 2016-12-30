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

/*参数是String类，并且参数有多个*/
int test_02()
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

    /*映射类中的方法, 该方法有几个参数，第三个参数中就有几个，每一个参数类型后面都有一个分号*/
    jmethodID func = (*env)->GetMethodID(env, cls, "test_02", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

    /*转化参数*/
    jstring arg1 = (*env)->NewStringUTF(env, "this is arg1");
    jstring arg2 = (*env)->NewStringUTF(env, "this is arg2");
    jstring arg3 = (*env)->NewStringUTF(env, "this is arg3");
    puts("GetString");

    /*调用方法*/
    (*env)->CallVoidMethod(env, obj, func, arg1, arg2, arg3);   
    puts("CallVoidMethod");

    (*jvm)->DestroyJavaVM(jvm);   
    
    return 0;   
}

/*参数为空，返回值为String类*/
int test_03()
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

    /*映射类中的方法, 该方法有几个参数，第三个参数中就有几个，每一个参数类型后面都有一个分号*/
    jmethodID func = (*env)->GetMethodID(env, cls, "test_03", "()Ljava/lang/String;");

    /*调用方法*/
    jstring jstr = (*env)->CallObjectMethod(env, obj, func);   
    puts("CallObjectMethod");


    /*将jstring转化成char*的方法*/
    jboolean is_copy = JNI_TRUE;
    const char *cstr = (*env)->GetStringUTFChars(env, jstr, &is_copy);
    printf("return_str --> %s\n", cstr);
    printf("return_str len = %d, is_copy = %d\n", (*env)->GetStringUTFLength(env, jstr), is_copy);

    /*释放cstr的内存*/
    (*env)->ReleaseStringUTFChars(env, jstr, cstr);

    /*关闭JVM*/
    (*jvm)->DestroyJavaVM(jvm);   
    
    return 0;   
}

int main(void)
{
    test_02();

    return 0;
}
