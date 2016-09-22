#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
    int n = 0;
    FILE *fp;

    while (1)
    {
        if ((fp = fopen("1.txt", "w")) == NULL)
        {
            fprintf(stderr, "fopen:%s\n", strerror(errno));
            printf("n=%d\n", n);
            return -1;
        }
        n++;
    }
    fclose(fp);

    return 0;
}
