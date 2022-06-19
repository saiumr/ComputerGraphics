#pragma once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdint>

class ALGOLine {
private:
    enum LineWay {
        kDDA,
        kMidPoint, 
        kBresenham
    };

    SDL_Point aim_point_, end_point_;
    unsigned int point_num_;
    unsigned int switch_way_count_;
    void (ALGOLine::*draw_line_way_)(SDL_Renderer*, int, int, int, int);
    const static uint8_t kTotalWays;

    void SwitchWay();

    void ChosePointByX(const SDL_Point& p1, const SDL_Point& p2);
    void ChosePointByY(const SDL_Point& p1, const SDL_Point& p2);
    void DrawLine_DDA(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
    void DrawLine_MidPoint(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
    void DrawLine_Bresenham(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);

public:
    ALGOLine();
    ~ALGOLine();

    void DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
    void EventHandle(SDL_Event& event, SDL_Renderer* renderer);
};


