#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#      include <termios.h>
#include <unistd.h>
#      define sgttyb termios
#      define sg_flags c_lflag
#      define GTTY(f, s) tcgetattr(f, (void *) s)
#      define STTY(f, s) tcsetattr(f, TCSAFLUSH, (void *) s)

static int echofd=(-1);      /* file descriptor whose echo is off */

/*
 * Turn echo off for file descriptor f.  Assumes that f is a tty device.
 */
void Echoff(f)
    int f;                    /* file descriptor for which to turn echo off */
{
    struct sgttyb sg;         /* tty device structure */

    echofd = f;
    GTTY(f, &sg);             /* get settings */
    sg.sg_flags &= ~ECHO;     /* turn echo off */
    STTY(f, &sg);
}

/*
 * Turn echo back on for file descriptor echofd.
 */
void Echon()
{
    struct sgttyb sg;         /* tty device structure */

    if (echofd != -1) {
        GTTY(echofd, &sg);    /* get settings */
        sg.sg_flags |= ECHO;  /* turn echo on */
        STTY(echofd, &sg);
        echofd = -1;
    }
}

#    define _PATH_TTY "/dev/tty"
/*
 * 该函数实现了类似于scanf函数的功能，从标准输入中获取信息
 * 1、打开设备"/dev/tty"，获取文件描述符f
 * 2、将密码信息写入到stderr中
 * 3、打开文件描述符f的回显标志
 * 4、从文件描述符f中读取信息，每次读取1个字节，然后将读取的信息赋值给p 
 * */
char *getp(m, p, n)
    const char *m;             /* prompt for password */
    char *p;                    /* return value: line input */
    int n;                      /* bytes available in p[] */
{
    char c;                     /* one-byte buffer for read() to use */
    int i;                      /* number of characters input */
    char *w;                    /* warning on retry */
    int f;                      /* file descriptor for tty device */

    /* turn off echo on tty */

    if ((f = open(_PATH_TTY, 0)) == -1)
        return NULL;
    /* get password */
    w = "";
    do {
        fputs(w, stderr);       /* warning if back again */
        fputs(m, stderr);       /* prompt */
        fflush(stderr);
        i = 0;
        Echoff(f);
        do {                    /* read line, keeping n */
            read(f, &c, 1);
            if (i < n)
                p[i++] = c;
        } while (c != '\n');
        Echon();
        putc('\n', stderr);  fflush(stderr);
        w = "(line too long--try again)\n";
    } while (p[i-1] != '\n');
    p[i-1] = 0;                 /* terminate at newline */
    printf("getp, p=[%s]\n", p);

    close(f);

    return p;                   /* return pointer to password */

} /* end function getp() */
int main(int argc, const char *argv[])
{
    char buf[81] = "Enter password: ";
    char passwd[81] = {0};
    if( NULL == getp(buf, passwd, 81) )
    {
        fprintf(stderr, "getp error\n");
        return -1;
    }
    printf("passwd=[%s]\n", passwd);
    return -1;
}
