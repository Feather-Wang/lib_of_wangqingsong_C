#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iniparser.h>

int main()
{
    FILE *fp;
    const char *value;
    dictionary *dic = NULL;
    char *filepath = "commoninfo.ini";
    int ret = -1;

    do{
        dic = iniparser_load(filepath);
        if( NULL == dic )
        {
            printf("load file [%s] is Error\n", filepath);
            break;
        }

        /* 获取指定的值 */
        /* 这里的value不用释放，返回的是dic里的指针，在最后释放dic的时候会一并释放 */
        value = iniparser_getstring(dic, "Device:Name", (char *)NULL);
        if( NULL == value )
        {
            printf("can't find device:name\n");
        }
        else
        {
            if( 0 >= strlen(value))
            {
                printf("there isn't value with device:name\n");
            }
            else
            {
                printf("name: %s\n", value);
            }
        }
        /* 修改指定的值 */
        iniparser_set(dic, "Device:Name", "");

        fp = fopen(filepath, "w+");
        if( NULL == fp )
        {
            printf("open file [%s] is Error\n", filepath);
            break;
        }
        /* 将修改后的值写入到文件中 */
        iniparser_dump_ini(dic, fp);

        ret = 0;

    }while(0);

    if(fp)
        fclose(fp);
    if(dic)
        iniparser_freedict(dic);

    return 0;
}
