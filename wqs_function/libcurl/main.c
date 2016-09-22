#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_url.h"

int main(int argc, const char *argv[])
{
    //char url[128] = "http://192.168.91.43:22";
    char url[1280] = "www.baidu.com";
    char databuff[102400] = {0};

    CURL *curl = curl_init();

    curl_load(curl, url, databuff);

    curl_send(curl);

    double run_time;
    curl_get_run_time(curl, &run_time);
    printf("run_time = %f\n", run_time);
    printf("databuf -->\n%s\n", databuff);

    curl_clean(curl);
    
    return 0;
}
