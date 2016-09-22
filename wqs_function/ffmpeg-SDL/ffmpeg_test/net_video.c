#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//注册av_read_frame的回调函数，这里只是最简处理，实际应用中应加上出错处理，超时等待...
int read_data(void *opaque, uint8_t *buf, int buf_size) 
{
    FILE *fp = (FILE*)opaque;
    size_t size = fread(buf, 1, buf_size, fp);
    int ret = size;
    return ret;
}
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 1880
#define BUF_SIZE 1024*1024
int main(int argc, char** argv) 
{
	uint8_t *buf = (uint8_t*)malloc(sizeof(uint8_t)*BUF_SIZE);

	av_register_all();
	AVCodec *pVideoCodec, *pAudioCodec;
	AVCodecContext *pVideoCodecCtx = NULL;
	AVCodecContext *pAudioCodecCtx = NULL;
	AVIOContext * pb = NULL;
	AVInputFormat *piFmt = NULL;
	AVFormatContext *pFmt = NULL;
    puts("111111111111111111");

    FILE *fp = fopen("./file.mp4", "rb");
	//step1:申请一个AVIOContext
	pb = avio_alloc_context(buf, BUF_SIZE, 0, fp, read_data, NULL, NULL);
	if (!pb) {
		fprintf(stderr, "avio alloc failed!\n");
		return -1;
	}
    puts("2222222222222222222");
	//step2:探测流格式
	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0) {
		fprintf(stderr, "probe failed!\n");
		return -1;
	} else {
        puts("22222223333333");
		fprintf(stdout, "probe success!\n");
		fprintf(stdout, "format: %s[%s]\n", piFmt->name, piFmt->long_name);
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
	if (av_find_stream_info(pFmt) < 0) {
		fprintf(stderr, "could not fine stream.\n");
		return -1;
	}
    puts("******************************************11111111111");
	av_dump_format(pFmt, 0, "", 0);
    puts("******************************************22222222222");
    sleep(2);
	int videoindex = -1;
	int audioindex = -1;
    int i = 0;
	for (i = 0; i < pFmt->nb_streams; i++) {
		if ( (pFmt->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) &&
				(videoindex < 0) ) {
			videoindex = i;
		}
		if ( (pFmt->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) &&
				(audioindex < 0) ) {
			audioindex = i;
		}
	}
	if (videoindex < 0 || audioindex < 0) {
		fprintf(stderr, "videoindex=%d, audioindex=%d\n", videoindex, audioindex);
		return -1;
	}
	AVStream *pVst,*pAst;
	pVst = pFmt->streams[videoindex];
	pAst = pFmt->streams[audioindex];
	pVideoCodecCtx = pVst->codec;
	pAudioCodecCtx = pAst->codec;
	pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
	if (!pVideoCodec) {
		fprintf(stderr, "could not find video decoder!\n");
		return -1;
	}
    sleep(2);
	if (avcodec_open2(pVideoCodecCtx, pVideoCodec, NULL) < 0) {
		fprintf(stderr, "could not open video codec!\n");
		return -1;
	}
	pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
	if (!pAudioCodec) {
		fprintf(stderr, "could not find audio decoder!\n");
		return -1;
	}
	if (avcodec_open2(pAudioCodecCtx, pAudioCodec, NULL) < 0) {
		fprintf(stderr, "could not open audio codec!\n");
		return -1;
	}
	int got_picture;
	uint8_t samples[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
	AVFrame *pframe = avcodec_alloc_frame();
	AVPacket pkt;
	av_init_packet(&pkt);
	while(1) {
		if (av_read_frame(pFmt, &pkt) >= 0) {
			if (pkt.stream_index == videoindex) {
				fprintf(stdout, "pkt.size=%d,pkt.pts=%lld, pkt.data=0x%x.", pkt.size, pkt.pts,(unsigned int)pkt.data);
				avcodec_decode_video2(pVideoCodecCtx, pframe, &got_picture, &pkt);
				if (got_picture) {
					fprintf(stdout, "decode one video frame!\n");
				}
			}else if (pkt.stream_index == audioindex) {
				int frame_size = AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2;
				if (avcodec_decode_audio3(pAudioCodecCtx, (int16_t *)samples, &frame_size, &pkt) >= 0) {
					fprintf(stdout, "decode one audio frame!\n");
				}
			}
			av_free_packet(&pkt);
		}
	}
	av_free(buf);
	av_free(pframe);
	return 0;
}

