#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/*
 *struct tm
 {
 int tm_sec;    //second ( 0~59 )
 int tm_min;    //minute ( 0~59 )
 int tm_hour;   //hour ( 0~23 )
 int tm_mday;   //day by month ( 1~31 )
 int tm_mon;    //month ( 1~12 )
 int tm_year;   //year ( the count of year since 1900 )
 int tm_wday;   //day by week ( 0~6 )
 int tm_yday;   //day by year ( 1~365 )
 int tm_isdst;  //夏令时标识符，实行夏令时的时候，tm_isdst为正，不实行夏令时的时候，tm_isdst为0，不了解情况时，tm_isdst为负
 long int tm_gmtoff; //指定日期变更线东面时区中UTC东部时区的正秒数或UTC西部时区的负秒数
 const char *tm_zone; //当前时区的名字（与环境变量TZ有关）
 }
 * */
/*
 *strftime：将struct tm结构体的数据转换成字符串
 参数
 第一个参数：用来接收转化后的时间字符串
 第二个参数：第一个参数的大小
 第三个参数：转化后的时间字符串的格式
 第四个参数：得到的struct tm结构体指针变量
 格式说明：
 具体格式说明，请见笔记
 * */

int get_time_1()
{
    time_t time_now;
    time(&time_now);
    printf("time = %d\n", time_now);
    
    struct tm now;
    localtime_r(&time_now, &now);
    printf("local time by asctime() is : %s\n", asctime(&now));
    printf("local time by ctime() is : %s\n", ctime(&time_now));
    printf("local time by struct tm is : %d-%d-%d %d:%d:%d %s\n", 
            now.tm_year+1900, 
            now.tm_mon+1, 
            now.tm_mday, 
            now.tm_hour, 
            now.tm_min, 
            now.tm_sec,
            now.tm_zone
            );
    
    char time_string_1[1024] = {0};
    strftime(time_string_1, sizeof(time_string_1), "%c---%Z", &now);
    printf("local time by strftime is : %s\n", time_string_1);
    strftime(time_string_1, sizeof(time_string_1), "%Y-%m-%d %H:%M:%S", &now);
    printf("local time by strftime is : %s\n", time_string_1);

    return 0;
}

int get_time_2()
{
    struct timeval time_now;
    gettimeofday( &time_now, NULL);
    printf("sec = %x, usec = %x\n", time_now.tv_sec, time_now.tv_usec);
}

int main(int argc, const char *argv[])
{
    get_time_1();
    get_time_2();

    return 0;
}
