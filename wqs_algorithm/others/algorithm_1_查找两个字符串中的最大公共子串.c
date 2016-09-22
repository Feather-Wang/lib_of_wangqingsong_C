#include <stdio.h>
#include <malloc.h>
#include <string.h>

void commandstring(char shortstring[], char longstring[])
{
    int i,j;
    char substring[256] = {0};
    
    for (i = strlen(shortstring); i > 0; i--) 
    {
        for (j = 0; j <= strlen(shortstring)-i; j++) 
        {
            memset(substring, 0x00, sizeof(substring));
            memcpy(substring, &shortstring[j], i);
            substring[i] = '\0';
            if (strstr(longstring, substring) != NULL) 
            {
                printf("substring = %s\n", substring);
                return;
            }
            
        }
    }
}

int main(int argc, const char *argv[])
{
    char str1[256] = "wabcq";
    char str2[256] = "habcdd";

    commandstring(str1, str2);
    return 0;
}
