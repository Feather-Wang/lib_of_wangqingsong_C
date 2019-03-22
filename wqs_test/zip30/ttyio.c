/*
   ttyio.c - Zip 3

   Copyright (c) 1990-2005 Info-ZIP.  All rights reserved.

   See the accompanying file LICENSE, version 2005-Feb-10 or later
   (the contents of which are also included in zip.h) for terms of use.
   If, for some reason, all these files are missing, the Info-ZIP license
   also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
   */
/*---------------------------------------------------------------------------

  ttyio.c

  This file contains routines for doing console input/output, including code
  for non-echoing input.  It is used by the encryption/decryption code but
  does not contain any restricted code itself.  This file is shared between
  Info-ZIP's Zip and UnZip.

Contains:  echo()         (VMS only)
Echon()        (Unix only)
Echoff()       (Unix only)
screensize()   (Unix only)
zgetch()       (Unix, VMS, and non-Unix/VMS versions)
getp()         ("PC," Unix/Atari/Be, VMS/VMCMS/MVS)

---------------------------------------------------------------------------*/

#define __TTYIO_C       /* identifies this source module */

#include "zip.h"
#include "crypt.h"

/* Non-echo console/keyboard input is needed for (en/de)cryption's password
 * entry, and for UnZip(SFX)'s MORE and Pause features.
 * (The corresponding #endif is found at the end of this module.)
 */

#include "ttyio.h"

#  define PUTC putc

#  define GLOBAL(g) g

#    define USE_POSIX_TERMIOS  /* use POSIX style termio (termios) */

/* include system support for switching of console echo */
#      include <termios.h>
#      define sgttyb termios
#      define sg_flags c_lflag
#      define GTTY(f, s) tcgetattr(f, (zvoid *) s)
#      define STTY(f, s) tcsetattr(f, TCSAFLUSH, (zvoid *) s)
#        include <fcntl.h>

static int echofd=(-1);      /* file descriptor whose echo is off */

/*
 * Turn echo off for file descriptor f.  Assumes that f is a tty device.
 */
void Echoff(__G__ f)
    __GDEF
    int f;                    /* file descriptor for which to turn echo off */
{
    struct sgttyb sg;         /* tty device structure */

    GLOBAL(echofd) = f;
    GTTY(f, &sg);             /* get settings */
    sg.sg_flags &= ~ECHO;     /* turn echo off */
    STTY(f, &sg);
}

/*
 * Turn echo back on for file descriptor echofd.
 */
void Echon(__G)
    __GDEF
{
    struct sgttyb sg;         /* tty device structure */

    if (GLOBAL(echofd) != -1) {
        GTTY(GLOBAL(echofd), &sg);    /* get settings */
        sg.sg_flags |= ECHO;  /* turn echo on */
        STTY(GLOBAL(echofd), &sg);
        GLOBAL(echofd) = -1;
    }
}


/*
 * Get a password of length n-1 or less into *p using the prompt *m.
 * The entered password is not echoed.
 */

#    define _PATH_TTY "/dev/tty"

/*
 * 该函数实现了类似于scanf函数的功能，通过标准输入获取信息
 * 1、打开设备"/dev/tty"，获取文件描述符f
 * 2、将密码信息写入到stderr中
 * 3、打开文件描述符f的回显标志
 * 4、从文件描述符f中读取信息，每次读取1个字节，然后将读取的信息赋值给p 
 * */
char *getp(__G__ m, p, n)
    __GDEF
    ZCONST char *m;             /* prompt for password */
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
        echoff(f);
        do {                    /* read line, keeping n */
            read(f, &c, 1);
            if (i < n)
                p[i++] = c;
        } while (c != '\n');
        echon();
        PUTC('\n', stderr);  fflush(stderr);
        w = "(line too long--try again)\n";
    } while (p[i-1] != '\n');
    p[i-1] = 0;                 /* terminate at newline */
    printf("getp, p=[%s]\n", p);

    close(f);

    return p;                   /* return pointer to password */

} /* end function getp() */
