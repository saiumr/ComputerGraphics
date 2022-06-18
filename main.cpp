#include <iostream>
#include "ALGOLine.h"
#include "SDL2/SDL_stdinc.h"

using namespace std;

// Computer Graphics Funtion
enum CGF {
    kDrawLine,
    kXScanner
};
const static Uint8 kCGFNum = 2;

int main(int argc, char* argv[]) {
    SDL_Window*     window = nullptr;
    SDL_Renderer*   renderer = nullptr;
    if ( SDL_Init(SDL_INIT_EVERYTHING) == 0 ) {
        window = SDL_CreateWindow("ComputerGraphics", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    SDL_Event event;
    bool is_quit = false;
    Uint8 switch_function = 0;

    ALGOLine draw_line;

    while (!is_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_q) {
                    is_quit = true;
                }
                if (event.key.keysym.sym == SDLK_c) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                }
                if (event.key.keysym.sym == SDLK_a) {
                    ++switch_function;
                }
            }
            switch (switch_function % kCGFNum) {
            case kDrawLine:
                draw_line.EventHandle(event, renderer);
                break;
            case kXScanner:
                break;
            default:
                break;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
