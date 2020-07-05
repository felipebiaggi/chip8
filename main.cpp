#include <iostream>
#include "Memoria.h"
#include <chrono>
#include <thread>
#include "stdint.h"
#include "SDL2/SDL.h"


uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

int main(int argc, char **argv) {

    // Command usage
    if (argc != 2) {
        std::cout << "Usage: chip8 <ROM file>" << std::endl;
        return 1;
    }

    int w = 1024;                   // Window width
    int h = 512;                    // Window height


    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }


    auto window = SDL_CreateWindow(
                "CHIP-8 Emulator",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                w, h, SDL_WINDOW_SHOWN
                );

    if (window == NULL){
        printf( "Window could not be created! SDL_Error: %s\n",
                SDL_GetError() );
        exit(2);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, w, h);

    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
                                                SDL_PIXELFORMAT_ARGB8888,
                                                SDL_TEXTUREACCESS_STREAMING,
                                                64, 32);

    uint32_t pixels[2048];

    Memoria memo;


    return 0;
}
