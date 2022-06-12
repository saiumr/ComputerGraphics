#pragma once

#include "SDL2/SDL_render.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdint>

class ALGOLine {
private:
    enum LineWay {
        kDDA,
        kMidPoint
    };

    SDL_Point p1_, p2_;
    SDL_Point aim_point_, end_point_;
    unsigned int point_num_;
    unsigned int switch_way_count_;
    void (ALGOLine::*draw_line_way_)(SDL_Renderer*);
    const static uint8_t kTotalWays;

    void ChosePointByX();
    void ChosePointByY();
    void DrawLine_DDA(SDL_Renderer* renderer);
    void DrawLine_MidPoint(SDL_Renderer* renderer);

public:
    ALGOLine();
    ~ALGOLine();

    void SwitchWay();
    void DrawLine(SDL_Renderer* renderer);
    void IncresePointNum();
    void set_p1(unsigned int x, unsigned int y);
    void set_p2(unsigned int x, unsigned int y);
    unsigned int get_point_num();
};


