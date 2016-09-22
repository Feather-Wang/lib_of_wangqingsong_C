#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/* 
 * 使用open打开一个文件后，会让文件描述符指向一个file结构体，并把引用计数+1
 * 使用dup和dup2复制文件描述符时，都是让新的文件描述符和原来的文件描述符指向同一个file结构体，并且引用计数为2，
 * 如果是使用open打开两次同一个文件，file结构体会创建两个，每个文件描述符指向不同的file结构体
 * 关闭copyfd或者fd，并不会关闭另一个，只是让file结构体的引用计数减1，当file结构体的引用计数为0时，才会关闭文件 
 * */
/*
 * 关于dup2()
 *  dup2()有两个参数，参数1：被复制的文件描述符，参数2：新的文件描述符的数值，也是dup2()的成功的返回值。该参数可以是数值，比如11，dup2()会就让数值为11的文件描述符指向参数1指向的file结构体，成功的话，将11最为返回值返回 
 *
 * */

int test_dup()
{
    puts("############################## test_dup ########################");
    int copyfd;

    char msg[] = "test dup : This is a test\n";
    char hello[] = "test dup : Hello World\n";
    char third[] = "test dup : this is third string\n";

    int fd = open("somefile", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if( fd < 0 )
    {
        puts("error");
        return -1;
    }

    //copyfd = dup(STDOUT_FILENO);
    copyfd = dup(fd);
    
    write(STDOUT_FILENO, msg, strlen(msg));

    write(fd, third, strlen(third));

    close(fd);

    write(copyfd, hello, strlen(hello));

    close(copyfd);
    

    return 0;
}

int test_dup2()
{
    puts("############################## test_dup 2 ########################");
    int fd, copyfd;

    char msg[] = "test dup 2 : This is a test\n";
    char msg2[] = "test dup 2 : This is a test 2\n";

    fd = open("somefile", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if( fd < 0 )
    {
        puts("error");
        return -1;
    }

    copyfd = dup2(fd, 100);
    printf("dup2 first copyfd = %d\n", copyfd);
    close(copyfd);
    copyfd = dup2(fd, STDOUT_FILENO);
    
    write(copyfd, msg, strlen(msg));
    write(fd, msg2, strlen(msg2));

    printf("test dup 2 : copyfd = %d\n", copyfd);
    printf("test dup 2 : fd = %d\n", fd);
    printf("test dup 2 : STDOUT_FILENO = %d\n", STDOUT_FILENO);

    close(copyfd);
    close(fd);

    return 0;
}

int main(int argc, const char *argv[])
{
    //test_dup();
    test_dup2();

    return 0;
}
