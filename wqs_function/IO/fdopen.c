#include <stdio.h>

int main(int argc, const char *argv[])
{
    FILE *fp = fdopen(1, "w");

    fprintf(fp, "Hello World!\n");

    fclose(fp);
    return 0;
}
