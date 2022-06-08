#pragma once

#include <SDL2/SDL.h>
#include <cmath>

class ALGOLine {
private:
    SDL_Point p1_, p2_;
    SDL_Point aim_point_, end_point_;
    unsigned int point_num_;

    void _DDA_ChosePointByX();
    void _DDA_ChosePointByY();
public:
    ALGOLine();
    ~ALGOLine();
    void DDA_DrawLine(SDL_Renderer* renderer);
    
    void IncresePointNum();
    void set_p1(unsigned int x, unsigned int y);
    void set_p2(unsigned int x, unsigned int y);
    unsigned int get_point_num();
};


