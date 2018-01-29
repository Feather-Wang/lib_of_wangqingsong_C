class JavaToC{

    static 
    {
        System.loadLibrary("hello");
    }

    public native int helloworld();

    public JavaToC()
    {
    }

    public static void main(String[] str)
    {
        System.out.println("there will be run JavaToC JNI");
        JavaToC fun = new JavaToC();
        fun.helloworld();
    }
}
