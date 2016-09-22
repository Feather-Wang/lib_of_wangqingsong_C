#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SIZE_128        128

int get_string(char *data, char *argument, char *value)
{
    int ret = -1;
    char *pstr = NULL;
    do{
        if( !(pstr = strstr(data, argument)) )
            break;

        if( !(pstr = strstr(pstr, ":")) )
            break;

        if( !(pstr = strstr(pstr, "\"")) )
            break;

        sscanf(pstr+1, "%[^\"]", value);

        ret = 0;

    }while(0);

    return ret;
}

int get_int(char *data, char *argument, int *value)
{
    int result = -1;

    char *pstr = NULL;
    do{
        if( !(pstr = strstr(data, argument)) )
            break;

        if( !(pstr = strstr(pstr, ":")) )
            break;

        sscanf(pstr+1, "%d", value);

        result = 0;

    }while(0);

    return result;
}

int main(void)
{                 
    printf("start test!!!!!!\n");
     
    char buf[] = "{\"retCode\": \"501001\", \"value\":10, \"retMsg\":\"request start is null\"}";

    char result[1024] = {0};
    int value = 0;

    if( -1 == get_string(buf, "retCode", result) )
    {
        printf("[%s:%d] get error\n", __func__, __LINE__);
        return -1;
    }

    if( -1 == get_int(buf, "value", &value) )
    {
        printf("[%s:%d] get error\n", __func__, __LINE__);
        return -1;
    }
        
    printf("[%s:%d] retCode = %s\n", __func__, __LINE__, result);
    printf("[%s:%d] value = %d\n", __func__, __LINE__, value);
    return 0;     
}             
