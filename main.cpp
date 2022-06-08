#include <iostream>
#include "ALGOLine.h"

using namespace std;

// 中点画线法
// 利用直线的一般式方程 F(x, y) = Ax + By + C
// k = -(A/B), A = -(∆y), B = ∆x, C = -B(∆x) -> 由两点式转换得知的
// 把中点坐标 M(x+1, y+0.5) 带入方程 d = F(x+1, y+0.5)
// d>=0 -> y = y+1, d<0 -> y = y
// d1 = d0+A+B (d0<0), d1 = d0+A(d>=0), d0=A+0.5B

int main(int argc, char* argv[]) {
    SDL_Window*     window = nullptr;
    SDL_Renderer*   renderer = nullptr;
    if ( SDL_Init(SDL_INIT_EVERYTHING) == 0 ) {
        window = SDL_CreateWindow("byo boy", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    SDL_Event event;
    bool is_quit = false;
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
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                draw_line.IncresePointNum();
                if (draw_line.get_point_num() % 2 == 0) {
                    draw_line.set_p2(event.motion.x, event.motion.y);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    draw_line.DDA_DrawLine(renderer);
                }
                else {
                    draw_line.set_p1(event.motion.x, event.motion.y);
                }
            }
        }

        //SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
