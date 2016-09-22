#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wqs_signal.h"

int main(int argc, const char *argv[])
{
    //test_recv_signal_4(SIGINT); 
    
    //test_recv_signal_1(SIG_BLOCK, SIGINT); 

    //test_recv_signal_2(); 

    //test_send_signal_1(5);

    test_send_signal_2(2, 0, 5, 0);

    //test_send_signal_4(SIGALRM);

    //test_recv_signal_3();

    //test_send_signal_3(getpid(), SIGALRM);

    //test_recv_signal_4(SIGALRM);
    //test_recv_signal_5(SIGINT);

    test_recv_signal_6();

    int i = 0;
    for( i = 0; i < 20; ++i)
    {
        printf("for index = %d\n", i);
#if 0
        struct itimerval it;
        memset(&it, 0x00, sizeof(it));
        getitimer(ITIMER_REAL, &it);
        printf("interval.tv_sec = %d, it_value.tv_sec = %d\n", 
                it.it_interval.tv_sec, it.it_value.tv_sec);
#endif
#if 1
        if(i == 3)
            test_send_signal_6(SIGINT);
            //test_send_signal_5(getpid(), SIGALRM);
            //test_send_signal_4(SIGALRM);
#endif
        puts("================================");
        sleep(1);
    }

    return 0;
}
