#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define N 1024

typedef struct gps_info{
    char value_n[16];
    char value_e[16];
    char value_h[16];
    char value_s[16];
}GPS_INFO;

int set_gps(char *buf, GPS_INFO *gps_info)
{
    int num_dou = 0;
    int num_star = 0;
    int num_flag = 0;
    char *pstr = NULL, *prev = buf;
    do
    {
        if( NULL != (pstr = strstr(prev, ",")) )
        {
            ++num_dou;
            if( num_dou == 2)
            {
                sscanf(pstr+1, "%[^,]", gps_info->value_n);
            }
            else if( num_dou == 4)
            {
                sscanf(pstr+1, "%[^,]", gps_info->value_e);
            }
            else if( num_dou == 9)
            {
                sscanf(pstr+1, "%[^,]", gps_info->value_h);
            }
        }else if( NULL != (pstr = strstr(prev, "*")) && num_dou == 14 )
        {
            ++num_star;
            break;
        }
        ++num_flag;
        prev = pstr+1;
        if( num_flag >= 15 )
            break;
    }while(1);
    printf("flag = %d, dou = %d, star = %d\n", num_flag, num_dou, num_star);

    if( num_flag != 14 || num_dou != 14 && num_star != 1 )
        return -1;

    return 0;
}

int get_gps_info(char *gps_file, GPS_INFO *gps_info)
{
    int return_value = -1;
    int fdr;
    unsigned char buf[1024] = {0};
    ssize_t buf_len;
    int is_gps = 0;


    printf("file --> %s\n", gps_file);
    if ((fdr = open(gps_file, O_RDONLY)) == -1)
    {
        perror("open for reading");
        return -1;
    }

    unsigned char tmp_char = 0;
    do
    {
        ssize_t tmp_num = 0;
        tmp_num = read(fdr, &tmp_char, 1);
        if( tmp_num <= 0)
        {
            fprintf(stderr, "read failed");
            break;
        }

        if( tmp_char == '$' )
        {
            is_gps = 1;
        }
        else if( tmp_char == '\n' )
        {
            printf("buf --> %s\n", buf);
            if( strstr(buf, "$GPGGA") )
            {
                if( -1 == set_gps(buf, gps_info) )
                    break;
                else
                {
                    return_value = 0;
                    //break;
                }
            }

            is_gps = 0;
            buf_len = 0;
            memset(buf, 0x00, N);
        }

        if( is_gps )
        {
            buf[buf_len] = tmp_char;
            ++buf_len;
        }

    }while(1);
    
    close(fdr);

    return return_value;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage:%s srcfile destfile\n", argv[0]);
        return 0;
    }

    GPS_INFO gps_info;
    memset(&gps_info, 0x00, sizeof(GPS_INFO));
    int res = 0;
    res = get_gps_info(argv[1], &gps_info);
    printf("res = %d, value_n = %s, value_e = %s, value_h = %s, value_s = %s\n",
            res, gps_info.value_n, gps_info.value_e, gps_info.value_h, gps_info.value_s);
    return 0;
}
