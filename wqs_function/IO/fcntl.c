#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

int test_1_F_DUPFD(const char *filename)
{
    puts("test_1_F_DUPFD ....................");

    char msg[] = "test dup : This is a test\n";
    char hello[] = "test dup : Hello World\n";
    char third[] = "test dup : this is third string\n";

    int newfd = 0;
    int fd = open(filename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if( fd < 0 )
    {
        puts("error");
        return -1;
    }

    //newfd = fcntl(fd, F_DUPFD, 0);
    newfd = fcntl(fd, F_DUPFD, 11);
    if( -1 == newfd )
    {
        perror("F_DUPFD Error : ");
        return -1;
    }
    
    printf("fd = %d\n", fd);
    printf("newfd = %d\n", newfd);

    write(fd, third, strlen(third));

    close(fd);

    write(newfd, hello, strlen(hello));

    close(newfd);

    return 0;
}

int test_2_F_GETFD_F_SETFD(const char *filename)
{
    puts("test_2_F_GETFD_FD_SETFD ...................");
    int fd = open(filename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if( fd < 0 )
    {
        puts("error");
        return -1;
    }

    unsigned int flag = fcntl(fd, F_GETFD);
    printf("flag = %x\n", flag);
    
    flag |= FD_CLOEXEC;

    fcntl(fd, F_SETFD, flag);

    flag = fcntl(fd, F_GETFD);
    printf("flag = %x\n", flag);

    close(fd);

    return 0;
}

int test_3_F_GETFL_F_SETFL(const char *filename)
{
    puts("test_3_F_GETFL_FD_SETFL ...............");
    int fd = open(filename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if( fd < 0 )
    {
        puts("error");
        return -1;
    }

    unsigned int flag = fcntl(fd, F_GETFL);
    printf("flag = %x\n", flag);
    
    flag |= O_APPEND;

    fcntl(fd, F_SETFL, flag);

    flag = fcntl(fd, F_GETFL);
    printf("flag = %x\n", flag);

    close(fd);

    return 0;
}

#include <sys/types.h>
#include <unistd.h>
int test_4_F_GETOWN_F_SETOWN(const char *filename)
{
    puts("test_4_F_GETOWN_F_SETOWN ...............");
    int fd = open(filename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if( fd < 0 )
    {
        puts("error");
        return -1;
    }

    pid_t pid = fcntl(fd, F_GETOWN);
    printf("pid 1 = %x\n", pid);
    
    pid = 0;
    pid = getpid();
    printf("getpid() = %x\n", pid);
    fcntl(fd, F_SETOWN, pid);

    pid = 0;
    pid = fcntl(fd, F_GETOWN);
    printf("pid 2 = %x\n", pid);

    close(fd);
   
    return 0;
}

int main(int argc, const char *argv[])
{
    int fd = 0;
    const char *filename = argv[1];

    test_1_F_DUPFD(filename);

    test_2_F_GETFD_F_SETFD(filename);

    test_3_F_GETFL_F_SETFL(filename);

    test_4_F_GETOWN_F_SETOWN(filename);
    
    return 0;
}
