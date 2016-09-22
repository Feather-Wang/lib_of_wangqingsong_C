#include <unistd.h>
#include <getopt.h>
#include <strings.h>
#include <stdio.h>

int reargu = 0;
int noargu = 0;
int opargu = 0;

static const struct option long_options[]=
{
    {"time",required_argument,&reargu,60},
    {"help",no_argument, &noargu,'?'},
    {"huo",optional_argument,&opargu,200},
    {NULL,0,NULL,0}
};

int main(int argc, char *argv[])
{
    int opt=0;
    int index=0;
    printf("t = %d, ? = %d, h = %d\n", 't', '?', 'h');

    while(EOF != (opt=getopt_long(argc,argv,"t:?h:",long_options,&index)) )
    {
        printf("index = %d, opt = %c\n", index, opt);
        printf("optind = %d, opterr = %d, optopt = %d, optarg = %s\n", optind, opterr, optopt, optarg);
        printf("reargu = %d, noargu = %d, opargu = %d\n", reargu, noargu, opargu);
        switch(opt)
        {
        case 't': puts(optarg);break;	     
        case '?': puts("help1");break;
        case 'h': puts(optarg);break;
        default:break;
        }

        puts("******************************************************************************");
    }

    printf("argc = %d, optind = %d\n", argc, optind);

    return 0;
}
