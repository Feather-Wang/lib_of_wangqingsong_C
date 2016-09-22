#include "wqs_character.h"

void Byte2Hex(const unsigned char *ByteBuf, unsigned int ByteLen, unsigned char* HexBuf, unsigned int HexSize)
{
    int i;
    for(i=0; i < ByteLen; ++i)
        snprintf(&(HexBuf[i*2]), HexSize-1, "%02X", ByteBuf[i]);
}

void Hex2Byte(const unsigned char *HexBuf, unsigned int HexLen, unsigned char* ByteBuf, unsigned int ByteSize)
{
    unsigned char tmp_char_1 = 0;
    unsigned char tmp_char_2 = 0;
    int i = 0;
    for(; i < HexLen && i/2 < ByteSize; i+=2)
    {
        if( 'A' <= HexBuf[i] && 'Z' >= HexBuf[i] )
        {
            tmp_char_1 = HexBuf[i] - 39;
        }
        else
        {
            tmp_char_1 = HexBuf[i];
        }

        if( 'A' <= HexBuf[i+1] && 'Z' >= HexBuf[i+1] )
        {
            tmp_char_2 = HexBuf[i+1] - 39;
        }
        else
        {
            tmp_char_2 = HexBuf[i+1];
        }

        //printf("Hex2Byte index = %d\n", i);
        ByteBuf[i/2] = (tmp_char_1 << 4 & 0xf0) | (tmp_char_2 & 0x0f);
    }
    ByteBuf[i/2] = '\0';
}

int strtok_string(char *buf, char *delim, char *argv[], int *len)
{
    int i = 0;

    /*分解第一个时，第一个参数要用要分解的字符串的指针，后面的几个，第一个参数就要用NULL*/
    argv[i] = strtok(buf,delim);
    while( (argv[++i] = strtok(NULL,delim)) )
        ;

    *len = i;

    return 0;
}
#if 1
int main(int argc, const char *argv[])
{
    unsigned char src[] = "100000";
    unsigned char Hex[1024] = {0};
    unsigned char Byte[1024] = {0};
    memset(Hex, 0x00, 1024);
    memset(Byte, 0x00, 1024);
    Byte2Hex(src, strlen(src), Hex, 1024);
    Hex2Byte(Hex, strlen(Hex), Byte, 1024);
    printf("src = %s\n", src);
    printf("Hex = %s\n", Hex);
    printf("Byte = %s\n", Byte);
    
    char str1[] = "3230313630313133313532343530313733";
    char buf[1024] = {0};
    Hex2Byte(str1, strlen(str1), buf, 1024);
    printf("buf -> %s\n", buf);

    char pstr_strtok[] = "a,b,c,d,e,f*g";
    char *result[24] = {NULL};
    int len = 0;
    strtok_string(pstr_strtok, ",*", result, &len);
    int i = 0;
    for(i = 0; i < len; i ++)
        printf(" %d : %s\n", i, result[i]);
    return 0;
}
#endif
