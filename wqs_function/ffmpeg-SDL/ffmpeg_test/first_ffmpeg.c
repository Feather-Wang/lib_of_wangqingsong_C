//cc test_ffmpeg.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lSDL -lpthread -lbz2 -lrt


#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

int img_convert(AVPicture *dst, enum PixelFormat dst_pix_fmt,const AVPicture *src, enum PixelFormat src_pix_fmt, int src_width,int src_height);

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
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            puts("1");
            pFrame = avcodec_alloc_frame();
            puts("1");
            int w = pCodecCtx->width;
            int h = pCodecCtx->height;
            // Decode video frame
            puts("2");
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);//�Ѱ�ת��Ϊ֡
            puts("2");
            puts("3");
            pSwsCtx = sws_getContext(w, h, pCodecCtx->pix_fmt, w, h,PIX_FMT_RGB565, SWS_POINT, NULL, NULL, NULL); //����õ���Ƶ�ֱ��ʣ�ɫ�ʿؼ��任ʱ�������ľ��
            puts("3");
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
            {
                fprintf(stderr, "Could not initialize SDL - %s/n",SDL_GetError());
                exit(1);
            }
            SDL_Surface *screen;
            puts("4");
            screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0,0);
            puts("4");
            if (!screen) 
            {
                fprintf(stderr, "SDL: could not set video mode - exiting/n");
                exit(1);
            }
            SDL_Overlay *bmp;
            puts("5");
            bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen);
            puts("5");
            SDL_Rect rect;
            if (frameFinished) 
            {
            puts("6");
                SDL_LockYUVOverlay(bmp);
            puts("6");
                AVPicture pict;
                pict.data[0] = bmp->pixels[0];
                pict.data[1] = bmp->pixels[2];
                pict.data[2] = bmp->pixels[1];
                pict.linesize[0] = bmp->pitches[0];//pitches��ָYUV�洢���ݶ�Ӧ��stride(����)
                pict.linesize[1] = bmp->pitches[2];
                pict.linesize[2] = bmp->pitches[1];
                // Convert the image into YUV format that SDL uses
                img_convert(&pict, PIX_FMT_YUV420P, (AVPicture *) pFrame,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
                SDL_UnlockYUVOverlay(bmp);
                rect.x = 0;
                rect.y = 0;
                rect.w = pCodecCtx->width;
                rect.h = pCodecCtx->height;
            puts("7");
                SDL_DisplayYUVOverlay(bmp, &rect);
            puts("7");
            }
            SDL_Event event;
            av_free_packet(&packet);
            SDL_PollEvent(&event);
            switch (event.type) {
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
            default:
                break;
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
