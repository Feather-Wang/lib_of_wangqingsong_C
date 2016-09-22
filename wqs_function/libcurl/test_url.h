#ifndef __TEST_URL_H__
#define __TEST_URL_H__
#include "libcurl/lib64/include/curl/curl.h"

CURL * curl_init();
int curl_load(CURL *curl, char *postURL, char *databuff);
int curl_send(CURL *curl);

#endif
