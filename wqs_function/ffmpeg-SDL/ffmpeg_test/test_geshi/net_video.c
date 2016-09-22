#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#define BUF_SIZE 1442*512
#if 1
//注册av_read_frame的回调函数，这里只是最简处理，实际应用中应加上出错处理，超时等待...
int copyBuf(void *opaque, uint8_t *buf, int buf_size) 
{
    FILE *p = (FILE *)opaque;

    fread(buf, 1, buf_size, p);
    return buf_size;
}
int main(int argc, char *argv[])
{
	uint8_t *buf = (uint8_t*)malloc(sizeof(uint8_t)*BUF_SIZE);

	av_register_all();
	AVIOContext * pb = NULL;
	AVInputFormat *piFmt = NULL;
	AVFormatContext *pFmt = NULL;
    puts("111111111111111111");

    FILE *fp = fopen(argv[1], "rb");

	pb = avio_alloc_context(buf, BUF_SIZE, 0, fp, copyBuf, NULL, NULL);
	if (!pb) {
		fprintf(stderr, "avio alloc failed!\n");
		return -1;
	}
    puts("2222222222222222222");
	//step2:探测流格式
	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 2048+1) < 0) {
	//if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0) {
		fprintf(stderr, "probe failed!\n");
		return -1;
	} else {
        puts("22222223333333");
		fprintf(stdout, "probe success!\n");
		fprintf(stdout, "format: %s[%s] --> ", piFmt->name, piFmt->long_name);
	}
    puts("252525252525252525");
	pFmt = avformat_alloc_context();
	pFmt->pb = pb; //step3:这一步很关键
    puts("33333333333333333333");
	//step4:打开流
	if (avformat_open_input(&pFmt, "", piFmt, NULL) < 0) {
		fprintf(stderr, "avformat open failed.\n");
		return -1;
	} else {
		fprintf(stdout, "open stream success!\n");
	}
    printf("444444444444444444\n");
	//以下就和文件处理一致了
#if 1
	if (avformat_find_stream_info(pFmt, NULL) < 0) {
		fprintf(stderr, "could not fine stream.\n");
		return -1;
	}
#endif
    puts("******************************************11111111111");
    printf("wangqingsong ------------->%\n");
    int wi = 0;
    char bufz[256] = {0};
    for(wi = 0; wi < pFmt->nb_streams; wi++ )
    {
        AVStream *st = pFmt->streams[wi];
        avcodec_string(bufz, sizeof(bufz), st->codec, 0);
        printf("-->%s\n", bufz);
    }
	av_dump_format(pFmt, 0, "", 0);
    puts("******************************************22222222222");

	free(buf);
	return 0;
}
#else
int copyBuf2(void *opaque, uint8_t *buf, int buf_size) 
{
    char *p = (char *)opaque;
    memcpy(buf, p, buf_size);
    return buf_size;
}

int jumpvideo(char *pbuf, int bufsize)
{
	uint8_t *buf = (uint8_t*)malloc(sizeof(uint8_t)*BUF_SIZE);

	av_register_all();
	AVIOContext * pb = NULL;
	AVInputFormat *piFmt = NULL;
	AVFormatContext *pFmt = NULL;
    puts("111111111111111111");

	pb = avio_alloc_context(buf, bufsize, 0, pbuf, copyBuf2, NULL, NULL);
	if (!pb) {
		fprintf(stderr, "avio alloc failed!\n");
		return -1;
	}
    puts("2222222222222222222");
	//step2:探测流格式
	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 2048+1) < 0) {
	//if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0) {
		fprintf(stderr, "probe failed!\n");
		return -1;
	} else {
        puts("22222223333333");
		fprintf(stdout, "probe success!\n");
		fprintf(stdout, "format: %s[%s] --> ", piFmt->name, piFmt->long_name);
	}
    puts("252525252525252525");
	pFmt = avformat_alloc_context();
	pFmt->pb = pb; //step3:这一步很关键
    puts("33333333333333333333");
	//step4:打开流
	if (avformat_open_input(&pFmt, "", piFmt, NULL) < 0) {
		fprintf(stderr, "avformat open failed.\n");
		return -1;
	} else {
		fprintf(stdout, "open stream success!\n");
	}
    printf("444444444444444444\n");
	//以下就和文件处理一致了
#if 1
	if (avformat_find_stream_info(pFmt, NULL) < 0) {
		fprintf(stderr, "could not fine stream.\n");
		return -1;
	}
#endif
    puts("******************************************11111111111");
    printf("wangqingsong ------------->%\n");
    int wi = 0;
    char bufz[256] = {0};
    for(wi = 0; wi < pFmt->nb_streams; wi++ )
    {
        AVStream *st = pFmt->streams[wi];
        avcodec_string(bufz, sizeof(bufz), st->codec, 0);
        printf("-->%s\n", bufz);
    }
	av_dump_format(pFmt, 0, "", 0);
    puts("******************************************22222222222");

	free(buf);
	return 0;
}
#endif
