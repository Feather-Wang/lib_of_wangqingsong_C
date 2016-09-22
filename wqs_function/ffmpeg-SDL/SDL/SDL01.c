#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


void pressESCtoQuit();

int main(int argc, char* argv[])
{
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        puts("SDL_INIT failed...");
        return 1;
    }
#if 1
    const int SCREEN_WIDTH = 0;    // 0 means use current width.
    const int SCREEN_HEIGHT = 0;    // 0 means use current height.
    const int SCREEN_BPP = 0;        // 0 means use current bpp.
#else
    const int SCREEN_WIDTH = 640;    // 0 means use current width.
    const int SCREEN_HEIGHT = 480;    // 0 means use current height.
    const int SCREEN_BPP = 32;        // 0 means use current bpp.
#endif
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

    pShownBMP = IMG_Load(argv[1]); // Load a BMP file, and convert it as a surface.
    //pShownBMP = SDL_LoadBMP("picture.bmp"); // Load a BMP file, and convert it as a surface.
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
