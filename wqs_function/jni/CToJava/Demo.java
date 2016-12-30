public class Demo
{
    public Demo()
    {
    }

    public void HelloWorld()
    {
        System.out.println("hello");
    }

    //public void test_02(String arg1, String arg2, String arg3)
    public void test_02(String arg1)
    {
        System.out.println("arg1 --> " + arg1);
    }

    public String test_03()
    {
        return new String("this is test_03");
    }

    public static void StaticHelloWorld()
    {
        System.out.println("this is a StaticHelloWorld");
    }

    public Object getFileInfo()
    {
        com.file.FileInfo fi = null;
        fi = new com.file.FileInfo("wang.txt", 123);
        return fi;
    }
}

