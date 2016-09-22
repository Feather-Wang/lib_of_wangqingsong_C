#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int make_srand(int range)
{
    struct timeval tpstart;
    int n = 0;
    static int seed = 0;

    gettimeofday( &tpstart, NULL);
    seed = tpstart.tv_usec;

    n = rand_r(&seed)%range;

    return n;
}

int main(int argc, const char *argv[])
{
    unsigned int n = 0;
    int nn = 0;

    for( nn = 0; nn < 100000; ++nn){

        n = make_srand(1000000);
        if(n < 100)
        printf("n = %d\n", n);
    }

}
