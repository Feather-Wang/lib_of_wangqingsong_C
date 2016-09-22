#include "test_url.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//当curl调用该回调函数时，可能一个回复会调用多次，比如只有HTTP头部的回复，每一个字段都会调用一次,所以在memcpy时，一定要计算上stream的偏移量，
static size_t writedata(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    size_t res_size;

    res_size = size * nmemb;
    //memcpy(stream, ptr, res_size);

    return res_size;
}

CURL * curl_init()
{
    CURL *curl = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    return curl;
}

int curl_load(CURL *curl, char *postURL, char *databuff)
{
    int time_out = 1;
    int connect_timeout = 10;

    curl_easy_setopt(curl, CURLOPT_URL, postURL);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, time_out);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writedata);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, databuff);

    return 0;
}

int curl_get_run_time(CURL *curl, double *run_time)
{
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, run_time);
    return 0;
}

void curl_clean(CURL *curl)
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

int curl_general_send(CURL *curl)
{

    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, "Accept:application/XML");
    slist = curl_slist_append(slist, "Content-Type: application/XML");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);

    int res = curl_easy_perform(curl);
    if( res != CURLE_OK )
    {
        if( res == CURLE_OPERATION_TIMEDOUT )
        {
            puts("time out .....");
        }
        printf("curl failed %s\n", curl_easy_strerror(res));
    }
    else
    {
        double datasize = 0.0;
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &datasize);
        printf("datasize = %lfMB\n", datasize/1024/1024);
    }

    if( NULL != slist )
    {
        curl_slist_free_all(slist);
    }

    return 0;
}

static long file_size(char *file)
{
    FILE *fp = fopen(file, "r");
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);

    fclose(fp);
    return filesize;
}
struct file_info
{
    FILE *fp;
    long file_size;
}
static size_t curl_upload_file_func(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    struct file_info *info = (struct file_info*)stream;
    size_t read_size = 0;
    if( info->file_size >= size*nmemb )
    {
        read_size = size * nmemb;
    }
    else
    {
        read_size = info->file_size;
    }
    info->file_size -= read_size;

    fgets(ptr, read_size, info->fp);

    return read_size;
}

int curl_upload_file(CURL *curl, char *filename)
{
    int ret = -1;
    struct curl_slist *list = NULL;
    CURLcode code;
    struct file_info info;
    memset(&info, 0x00, sizeof(info));

    do{
        if( -1 == access(filename, F_OK) )
        {
            printf("judge %s : %s\n", filename, strerror(errno));
            break;
        }

        list = curl_slist_append(list, "Content-Type: application/txt");
        list = curl_slist_append(list, "Expect:");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

#if 1
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        info.file_size = file_size(filename);
        info.fp = fopen(filename, "r");
        if( NULL == info.fp )
            break;
        curl_easy_setopt(curl, CURLOPT_READDATA, &info);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, curl_upload_file_func);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_size);
#else
        struct curl_httppost *post = NULL;
        struct curl_httppost *last = NULL;

        curl_formadd(&post, &last, CURLFORM_COPYNAME, "sendfile", CURLFORM_FILE, filename, CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "filename", CURLFORM_COPYCONTENTS, filename, CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "submit", CURLFORM_COPYCONTENTS, "send", CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);	
#endif
        code = curl_easy_perform(curl);

        if( CURLE_OK != code )
        {
            printf("%s : code = %d, failed: %s\n", __func__, code, curl_easy_strerror(code));
        }
        else
        {
            double upload_speed = 0.0L;
            double total_time = 0.0L;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            curl_easy_getinfo(curl,  CURLINFO_SPEED_UPLOAD, &upload_speed);
        }

        ret = 0;

    }while(0);

    if(info.fp)
        fclose(info.fp);
    if(list)
        curl_slist_free_all(list);

    return ret;
}
