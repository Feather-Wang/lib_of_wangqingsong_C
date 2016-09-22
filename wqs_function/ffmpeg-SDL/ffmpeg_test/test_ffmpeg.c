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
    AVFormatContext *pFormatCtx; //输入输出的容器，但不能同时作为输入和输出的容器
    int i, videoStream;
    AVCodecContext *pCodecCtx;   //动态的记录一个解码器的上下文
    AVCodec *pCodec;    //解码器是由链表组成的，链表的类型是AVCodec
    AVFrame *pFrame;    //用来保存数据缓存的对象
    AVFrame *pFrameRGB;
    AVPacket packet;    //主要记录音视频数据帧，时钟信息和压缩数据首地址，大小等信息
    int frameFinished = 0;
    int numBytes;
    uint8_t *buffer;
    struct SwsContext *pSwsCtx;  //视频分辨率，色彩空间变换时所需要的上下文句柄

    puts("00000000000000000000");
    av_register_all();
    //const char *filename = "./test10.h264";
    char filename[32] = {0};
    sprintf(filename, "%s", argv[1]);
    printf("video file name is %s...\n", filename);

    //读取文件的头部并且把信息保存到pFormatCtx    检测了文件的头部
    pFormatCtx=avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filename, NULL,NULL) != 0)
        return -1; 

    puts("111111111111111111");
    //从文件的头部，检查文件中的流的信息
    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
        return -1; // Couldn't find stream information

    puts("222222222222222222");
    //负责为pFormatCtx->streams填上正确的信息,pFormatCtx->streams仅仅是一组大小为pFormatCtx->nb_streams的指针
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

    // 得到解码器的上下文信息
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

    // 计算AVCodeContext缓冲区的大小和申请空间
    numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    //申请内存对齐
    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    //给pFrameRGB 分配内存
    avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

    puts("while start...");
    i = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);//把包转化为帧

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
            dst->linesize);//把RGB转化为image
    //这样释放掉pSwsCtx的内存

    return 0;
}
