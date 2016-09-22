#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>

#if 0
void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) 
{
    FILE *pFile;
    char szFilename[32];
    int  y;

    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
    fclose(pFile);
}
#endif
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
int SDL_picture(char *name)
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
    pShownBMP = SDL_LoadBMP(name); // Load a BMP file, and convert it as a surface.
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

    pressESCtoQuit();
    SDL_Quit();

    return 0;

}
void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) 
{
    FILE *pFile;
    char szFilename[32];
    int  y;

    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
    fclose(pFile);
}


int main(int argc, char *argv[]) 
{
    AVFormatContext *pFormatCtx = NULL;
    int             i, videoStream;
    AVCodecContext  *pCodecCtx = NULL;
    AVCodec         *pCodec = NULL;
    AVFrame         *pFrame = NULL; 
    AVFrame         *pFrameRGB = NULL;
    AVPacket        packet;
    int             frameFinished;
    int             numBytes;
    uint8_t         *buffer = NULL;

    AVDictionary    *optionsDict = NULL;
    struct SwsContext      *sws_ctx = NULL;

    if(argc < 2) {
        printf("Please provide a movie file\n");
        return -1;
    }
    // Register all formats and codecs
    av_register_all();

    // Open video file
    if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL)!=0)
        return -1; // Couldn't open file

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
        return -1; // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, argv[1], 0);

    // Find the first video stream
    videoStream=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
        return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }
    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)
        return -1; // Could not open codec

    // Allocate video frame
    pFrame=av_frame_alloc();

    // Allocate an AVFrame structure
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB==NULL)
        return -1;

    // Determine required buffer size and allocate buffer
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
            pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    sws_ctx =
        sws_getContext
        (
         pCodecCtx->width,
         pCodecCtx->height,
         pCodecCtx->pix_fmt,
         pCodecCtx->width,
         pCodecCtx->height,
         PIX_FMT_RGB24,
         SWS_BILINEAR,
         NULL,
         NULL,
         NULL
        );

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
            pCodecCtx->width, pCodecCtx->height);

    // Read frames and save first five frames to disk
    i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0) 
    {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) 
        {
            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, 
                    &packet);

            // Did we get a video frame?
            if(frameFinished) 
            {
                // Convert the image from its native format to RGB
                sws_scale
                    (
                     sws_ctx,
                     (uint8_t const * const *)pFrame->data,
                     pFrame->linesize,
                     0,
                     pCodecCtx->height,
                     pFrameRGB->data,
                     pFrameRGB->linesize
                    );

                // Save the frame to disk
                SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
    }

    // Free the RGB image
    av_free(buffer);
    av_free(pFrameRGB);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 0;
}
