#include <setjmp.h>
#include <stdio.h>
#include <malloc.h>

int main(int argc, const char *argv[])
{
    jmp_buf env;
    char *pstr = NULL;
    int i = 0;
    i = setjmp(env);

    if( NULL == pstr )
    {
        puts("malloc");
        pstr = (char*)malloc(sizeof(char)*24);
    }
    printf("%d : i = %d, pstr --> %p\n", __LINE__, i, pstr);

    sleep(2);

    longjmp(env, ++i);

    return 0;
}
#if 0
int proc_4(jmp_buf env, int i)
{
    if (i == 0) longjmp(env, 1);
    return 14 % i;
}

int proc_3(jmp_buf env, int i, int j)
{
    return proc_4(env, i) + proc_4(env, j);
}

int proc_2(jmp_buf env, int i, int j, int k)
{
    return proc_3(env, i, k) + proc_3(env, j, k);
}

int proc_1(jmp_buf env, int i, int j, int k, int l)
{
    return proc_2(env, i, j, k+1);
}


int main(int argc, char **argv)
{
    jmp_buf env;
    int sj;
    int i, j, k, l;

    if (argc != 5) {
        fprintf(stderr, "usage: sj2 i j k l\n");
        return -1;
    }

    sj = setjmp(env);
    if (sj != 0) {
        printf("Error -- bad value of i (%d), j (%d), k (%d), l (%d)\n", 
                i, j, k, l);
        return -1;
    }

    i = atoi(argv[1]);
    j = atoi(argv[2]);
    k = atoi(argv[3]);
    l = atoi(argv[4]);

    printf("proc_1(%d, %d, %d, %d) = %d\n", i, j, k, l, proc_1(env, i, j, k, l));

    return 0;
}
#endif
#if 0
int a(char *s, jmp_buf env)
{
    int i;
    i = setjmp(env);
    printf("Setjmp returned -- i = %d, 0x%x, %s\n", i, s, s);
    return i;
}
int b(int i, jmp_buf env)
{
    printf("In B: i=%d.  Calling longjmp(env, i)\n", i);
    longjmp(env, i);
}
int main(int argc, char **argv)
{
    jmp_buf env;
    if (a("Jim", env) != 0) 
        return -1;
    if (a("Jim", env) != 0) 
        return -1;
    b(3, env);

    return 0;
}
#endif
