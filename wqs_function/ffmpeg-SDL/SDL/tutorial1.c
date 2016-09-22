#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

void pressESCtoQuit()
{
    bool gameOver = false;
    while( gameOver == false )
    {
        SDL_Event gameEvent;
        while ( SDL_PollEvent(&gameEvent) != 0 )
        {
            if ( gameEvent.type == SDL_QUIT )
            {
                gameOver = true;
            }
            if ( gameEvent.type == SDL_KEYUP )
            {
                if ( gameEvent.key.keysym.sym == SDLK_ESCAPE )
                {
                    gameOver = true;
                }
            }
        }
    }
    return;
}

int printPicture(char *name)
{
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        puts("SDL_INIT failed...");
        return 1;
    }
    const int SCREEN_WIDTH = 640;    // 0 means use current width.
    const int SCREEN_HEIGHT = 480;    // 0 means use current height.
    const int SCREEN_BPP = 32;        // 0 means use current bpp.
    const Uint32 SCREEN_FLAGS = SDL_SWSURFACE;    // SDL_SWSURFACE == 0,surface in system memory.

    SDL_Surface* pScreen = 0;
    pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS);    // Creat a SDL window, and get the window's surface.
    if ( pScreen == 0 )
    {
        puts("SDL_SetVideoMode() failed!\n");
        SDL_Quit();
        return 1;
    }

    SDL_Surface* pShownBMP = 0;
    pShownBMP = IMG_Load(name); // Load a BMP file, and convert it as a surface.
    if ( pShownBMP == 0 )

    {
        puts("SDL_LoadBMP() failed!\n");
        SDL_Quit();
        return -1;
    }

    SDL_Rect* pSrcRect = 0;    // If pSrcRect is NULL, the entire source surface is copied. 

    SDL_Rect* pDstRect = 0;    // If pDstRect is NULL, then the destination position (upper left corner) is (0, 0).
    if ( SDL_BlitSurface(pShownBMP, pSrcRect, pScreen, pDstRect) != 0 )    // Put the BMP's surface on the SDL window's surface.
    {
        puts("SDL_BlitSurface() failed!\n");
        SDL_Quit();
        return -1;
    }

    if ( SDL_Flip(pScreen) != 0 )    // Show the SDL window's surface.
    {
        puts("SDL_Flip() failed!\n");
        SDL_Quit();
        return -1;
    }

    //    pressESCtoQuit();
    SDL_Quit();

    return 0;

}
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

    system("ls bmptest");
    puts("****************************************************");
    char picname[255] = {0};
    snprintf(picname, 255, "./bmptest/%03d.ppm", iFrame-1);
    printPicture(picname); 
    sleep(1);
    // Close file
    fclose(pFile);
}


int main(int argc, char **argv)
{
    AVFormatContext *pFormatCtx = NULL;
    int err, i;
    char *filename = "file.mp4"; // argv[1];
    AVCodec *pCodec = NULL;
    AVCodecContext *pCodecCtx;
    AVFrame *pFrame;
    AVFrame *pFrameRGB;
    uint8_t *buffer;
    int numBytes;
    int frameFinished;
    AVPacket packet;
    int videoStream;
    struct SwsContext *pSwsCtx; 

    av_log_set_level(AV_LOG_DEBUG);

    av_log(NULL, AV_LOG_INFO, "Playing: %s\n", filename);

    av_register_all();

    pFormatCtx = avformat_alloc_context();
    //    pFormatCtx->interrupt_callback.callback = decode_interrupt_cb;
    //    pFormatCtx->interrupt_callback.opaque = NULL;
    err = avformat_open_input(&pFormatCtx, filename, NULL, NULL);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "open_input fails, ret = %d\n", err);
        return -1;
    }

    err = avformat_find_stream_info(pFormatCtx, NULL);
    if (err < 0) {
        av_log(NULL, AV_LOG_WARNING, "could not find codec\n");
        return -1;
    }

    av_dump_format(pFormatCtx, 0, filename, 0);

    av_log(NULL, AV_LOG_INFO, "nb_streams in %s = %d\n", filename, pFormatCtx->nb_streams);
    videoStream = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream=i;
            av_log(NULL, AV_LOG_DEBUG, "video stream index = %d\n", i,
                    pFormatCtx->streams[i]->codec->codec_type);
            break;
        }
    }
    if(videoStream==-1) {
        av_log(NULL, AV_LOG_ERROR, "Haven't find video stream.\n");
        return -1; // Didn't find a video stream
    }

    // Find decoder
    pCodecCtx=pFormatCtx->streams[i]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec) {
        av_log(NULL, AV_LOG_ERROR, "%s: avcodec_find_decoder fails\n", filename);
        return -1;
    }

    // Open pCodec
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
        av_log(NULL, AV_LOG_ERROR, "%s: avcodec_open fails\n", filename);
        return -1; // Could not open codec
    }

    // Allocate video frame
    pFrame=avcodec_alloc_frame();
    if(pFrame == NULL)
        return -1;

    // Allocate an AVFrame structure
    pFrameRGB = avcodec_alloc_frame();
    if(pFrameRGB == NULL)
        return -1;

    // Determine required buffer size and allocate buffer
    numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
            pCodecCtx->width, pCodecCtx->height);

    pSwsCtx = sws_getContext (pCodecCtx->width,
            pCodecCtx->height,
            pCodecCtx->pix_fmt,
            pCodecCtx->width,
            pCodecCtx->height,
            PIX_FMT_RGB24,
            SWS_BICUBIC,
            NULL, NULL, NULL);
    i=0;
    while(av_read_frame(pFormatCtx, &packet) >= 0) 
    {
        if(packet.stream_index == videoStream) 
        { // Is this a packet from the video stream?
            avcodec_decode_video2(pCodecCtx,
                    pFrame,
                    &frameFinished,
                    &packet); // Decode video frame

            if(frameFinished) 
            { // Did we get a video frame?
                av_log(NULL, AV_LOG_DEBUG, "Frame %d decoding finished.\n", i);
                // Save the frame to disk
                if(i++ < 50) 
                {
                    //转换图像格式，将解压出来的YUV的图像转换为BRG24的图像
                    sws_scale(pSwsCtx,
                            pFrame->data,
                            pFrame->linesize,
                            0,
                            pCodecCtx->height,
                            pFrameRGB->data,
                            pFrameRGB->linesize);
                    // 保存为PPM
                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
                }
                else 
                {
                    break;
                }
            }
            else 
            {
                av_log(NULL, AV_LOG_DEBUG, "Frame not finished.\n");
            }
        }

        av_free_packet(&packet); // Free the packet that was allocated by av_read_frame
    }
    sws_freeContext (pSwsCtx);

    av_free (pFrame);
    av_free (pFrameRGB);
    av_free (buffer);
    avcodec_close (pCodecCtx);
    av_close_input_file (pFormatCtx);
}
