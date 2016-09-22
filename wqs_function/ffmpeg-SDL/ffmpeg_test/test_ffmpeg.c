//cc test_ffmpeg.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lSDL -lpthread -lbz2 -lrt


#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

int img_convert(AVPicture *dst, enum PixelFormat dst_pix_fmt,const AVPicture *src, enum PixelFormat src_pix_fmt, int src_width,int src_height);

static void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
    FILE *pFile;
    char szFilename[255];
    int  y;
 
    // Open file
    memset(szFilename, 0, sizeof(szFilename));
    snprintf(szFilename, 255, "./bmptest/%03d.ppm", iFrame);
    system("mkdir -p ./bmptest");
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;
 
    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
 
    // Write pixel data
    for(y = 0; y < height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
 
    // Close file
    fclose(pFile);
}

int main(int argc, char *argv[]) 
{
    puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    AVFormatContext *pFormatCtx; //���������������������ͬʱ��Ϊ��������������
    int i, videoStream;
    AVCodecContext *pCodecCtx;   //��̬�ļ�¼һ����������������
    AVCodec *pCodec;    //����������������ɵģ������������AVCodec
    AVFrame *pFrame;    //�����������ݻ���Ķ���
    AVFrame *pFrameRGB;
    AVPacket packet;    //��Ҫ��¼����Ƶ����֡��ʱ����Ϣ��ѹ�������׵�ַ����С����Ϣ
    int frameFinished = 0;
    int numBytes;
    uint8_t *buffer;
    struct SwsContext *pSwsCtx;  //��Ƶ�ֱ��ʣ�ɫ�ʿռ�任ʱ����Ҫ�������ľ��

    puts("00000000000000000000");
    av_register_all();
    //const char *filename = "./test10.h264";
    char filename[32] = {0};
    sprintf(filename, "%s", argv[1]);
    printf("video file name is %s...\n", filename);

    //��ȡ�ļ���ͷ�����Ұ���Ϣ���浽pFormatCtx    ������ļ���ͷ��
    pFormatCtx=avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filename, NULL,NULL) != 0)
        return -1; 

    puts("111111111111111111");
    //���ļ���ͷ��������ļ��е�������Ϣ
    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
        return -1; // Couldn't find stream information

    puts("222222222222222222");
    //����ΪpFormatCtx->streams������ȷ����Ϣ,pFormatCtx->streams������һ���СΪpFormatCtx->nb_streams��ָ��
    av_dump_format(pFormatCtx, 0, filename, 0);

    // Find the first video stream
    videoStream = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    if (videoStream == -1)
        return -1; // Didn't find a video stream

    // �õ�����������������Ϣ
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }
    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
        return -1; // Could not open codec

    // Allocate video frame
    pFrame = avcodec_alloc_frame();

    // Allocate an AVFrame structure
    pFrameRGB = avcodec_alloc_frame();
    if (pFrameRGB == NULL)
        return -1;

    // ����AVCodeContext�������Ĵ�С������ռ�
    numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    //�����ڴ����
    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    //��pFrameRGB �����ڴ�
    avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

    puts("while start...");
    i = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);//�Ѱ�ת��Ϊ֡

            if( frameFinished )
            {
                img_convert( (AVPicture *)pFrameRGB, PIX_FMT_RGB24, (AVPicture*)pFrame, pCodecCtx->pix_fmt,
                        pCodecCtx->width, pCodecCtx->height);

                if( ++i <= 5 )
                {
                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
                }
            }
        }
    }

    av_free(buffer);
    av_free(pFrameRGB);

    av_free(pFrame);

    avcodec_close(pCodecCtx);

    avformat_close_input(&pFormatCtx);

    return 0;
}

int img_convert(AVPicture *dst, enum PixelFormat dst_pix_fmt,const AVPicture *src, enum PixelFormat src_pix_fmt, int src_width,int src_height)
{
    int w;
    int h;
    struct SwsContext *pSwsCtx;

    w = src_width;
    h = src_height;
    pSwsCtx = sws_getContext(w, h, src_pix_fmt, w, h, dst_pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

    sws_scale(pSwsCtx, (const uint8_t * const *)src->data, src->linesize, 0, h, dst->data,
            dst->linesize);//��RGBת��Ϊimage
    //�����ͷŵ�pSwsCtx���ڴ�

    return 0;
}
