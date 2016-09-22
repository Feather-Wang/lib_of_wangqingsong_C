#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <idna.h>

int main(int argc, const char *argv[])
{
    char addr[] = "www.baidu.com";
    int ret = 0;
    char *recv = NULL;

    ret = idna_to_ascii_lz( addr, &recv, 0 );
    if( IDNA_SUCCESS != ret )
    {
        fprintf(stderr, "error\n");
        return -1;
    }
    fprintf(stdout, "recv : [ %s ]\n", recv);
    return 0;
}
