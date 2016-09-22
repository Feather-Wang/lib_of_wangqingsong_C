#include <stdio.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void showfile(char *filename);
void showdir(char *filename);

int main(int argc, char *argv[])
{
    struct stat buf;

    if (argc < 2)
    {
        showdir(".");
        return 0;
    }

    if (-1 == lstat(argv[1], &buf))
    {
        perror("lstat");
        return -1;
    }

    // 判断文件是否为目录文件
    if (S_ISDIR(buf.st_mode))//argv[1] d
    {
        showdir(argv[1]);
    }
    else
        showfile(argv[1]);

    return 0;
}

// 如果不是目录文件，显示该文件的信息
void showfile(char *filename)
{
    struct stat buf;
    struct tm *p;
    int i = 8;

    if (-1 == lstat(filename, &buf))
    {
        perror("lstat");
        return;
    }
    
    // 获取文件类型
    switch (buf.st_mode & S_IFMT)//0170000
    {
        case S_IFBLK: printf("b"); break;
        case S_IFCHR: printf("c"); break;
        case S_IFDIR: printf("d"); break;
        case S_IFREG: printf("-"); break;
        case S_IFLNK: printf("l"); break;
        case S_IFSOCK: printf("s"); break;
        case S_IFIFO: printf("p"); break;
    }

    // 获取读写权限信息，输出到屏幕上
    while (i >= 0)
    {
        if (buf.st_mode & 1 << i)
        {
            switch (i % 3)
            {
                case 2: printf("r"); break;
                case 1: printf("w"); break;
                case 0: printf("x"); break;
            }
        }
        else
            printf("-");
        i--;
    }


    // 获取文件最后修改时间
    p = localtime(&buf.st_mtime);

    printf("  %d  %s  %s  %d  %d-%d-%d  %d:%d  %s", 
            buf.st_nlink, 
            getpwuid(buf.st_uid)->pw_name, 
            getgrgid(buf.st_gid)->gr_name,
            (int)buf.st_size,
            p->tm_year+1900, p->tm_mon+1, p->tm_mday,
            p->tm_hour, p->tm_min, filename);

    printf("\n");

}

// 如果是目录文件，显示该目录下的所有文件的信息,如果存在子目录
// 不继续深入，只显示该子目录文件的信息
void showdir(char *filename)
{
        DIR *dp;
        struct dirent *ditem;
        char s[50];

        // 打开目录文件
        if ((dp = opendir(filename)) == NULL)
        {
            perror("opendir");
            return;
        }

        // 读取目录下的所有文件名
        //while ((ditem = readdir(dp)) != NULL)
        struct dirent entry;
        while ( readdir_r(dp, &entry, &ditem) == 0 && ditem != NULL)
        {
            printf("entry->d_name = %s\n", entry.d_name);
            // 过滤掉隐藏文件
            if (ditem->d_name[0] == '.')
                continue;
            
            sprintf(s, "%s/%s", filename, ditem->d_name);
            //printf("%s\n", s);//dir/a

            // 使用显示文件的函数，显示该文件信息
            showfile(s);//  dir/a
        }

        closedir(dp);
}
