#include "wqs_ffmpeg.h"

int i = 0;

//注册av_read_frame的回调函数，这里只是最简处理，实际应用中应加上出错处理，超时等待...
static int copyBuf(void *opaque, uint8_t *buf, int buf_size) 
{
    TSBUF *tsbuf = (TSBUF *)opaque;

    P(&r_rs);

    memcpy(buf, tsbuf->tsbuf, tsbuf->tslen);
    printf("wqs_ffmpeg ---> %d:%d...\n", i++, tsbuf->tslen);

    V(&w_rs);

    return tsbuf->tslen;
}

void *wqs_ffmpeg(void *arg)
{
    uint8_t *buf = (uint8_t*)malloc(sizeof(uint8_t)*TS_SIZE);

	av_register_all();
	AVIOContext * pb = NULL;
	AVInputFormat *piFmt = NULL;
	AVFormatContext *pFmt = NULL;

    //FILE *fp = fopen(argv[1], "rb");
	
    puts("avio_alloc_context start");
    pb = avio_alloc_context(buf, TS_SIZE, 0, arg, copyBuf, NULL, NULL);
	if (!pb) 
    {
		fprintf(stderr, "avio alloc failed!\n");
		return;
    }

	//step2:探测流格式
    puts("av_probe_input_buffer start");
	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0) 
    {
		fprintf(stderr, "probe failed!\n");
		return;
	}
    else
    {
		fprintf(stdout, "probe success!\n");
		fprintf(stdout, "format: %s[%s] --> ", piFmt->name, piFmt->long_name);
	}

    puts("avformat_alloc_context start");
	pFmt = avformat_alloc_context();
	pFmt->pb = pb; //step3:这一步很关键

    while( 1 )
    {
        //step4:打开流
        puts("avformat_open_input start");
        if (avformat_open_input(&pFmt, "", piFmt, NULL) < 0) 
        {
            fprintf(stderr, "avformat open failed.\n");
            avformat_free_context(pFmt);
            return;
        } 
        else 
        {
            fprintf(stdout, "open stream success!\n");
        }

        //以下就和文件处理一致了

        printf("probesize ---> %d, max ---> %d, max2 ---> %d...\n", pFmt->probesize, pFmt->max_analyze_duration, pFmt->max_analyze_duration2);
        //pFmt->probesize = 1442;
        //pFmt->max_analyze_duration = AV_TIME_BASE/100;
        //pFmt->max_analyze_duration2 = AV_TIME_BASE/100;

        printf("probesize ---> %d, max ---> %d, max2 ---> %d...\n", pFmt->probesize, pFmt->max_analyze_duration, pFmt->max_analyze_duration2);
        printf("------------find stream info begin------------ i = %d...\n", i);
        if (avformat_find_stream_info(pFmt, NULL) < 0) 
        {
            fprintf(stderr, "could not fine stream.\n");
            avformat_free_context(pFmt);
            return;
        }
        printf("------------find stream info end------------ i = %d...\n", i);

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
        avformat_free_context(pFmt);
    }
    free(buf);
	return;
}
