#include "ALGOLine.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include <cstdint>
#include <iostream>
#include <iterator>

const uint8_t ALGOLine::kTotalWays= 2;

ALGOLine::ALGOLine()  { 
    point_num_ = 0;
    switch_way_count_ = 0;
    draw_line_way_ = &ALGOLine::DrawLine_DDA;
}

ALGOLine::~ALGOLine() { }

void ALGOLine::SwitchWay() {
    ++switch_way_count_;
    switch (switch_way_count_ % kTotalWays) {
    case kDDA:
        draw_line_way_ = &ALGOLine::DrawLine_DDA;
        break;
    case kMidPoint:
        draw_line_way_ = &ALGOLine::DrawLine_MidPoint;
        break;
    }
}

void ALGOLine::DrawLine(SDL_Renderer* renderer) {
    (this->*draw_line_way_)(renderer);
}

// 选择x值小的作为起始点 0<=|k|<=1
void ALGOLine::ChosePointByX() {
    if (p1_.x < p2_.x) {
        aim_point_.x = p1_.x;
        aim_point_.y = p1_.y;
        end_point_.x = p2_.x;
        end_point_.y = p2_.y;
    }
    else {
        aim_point_.x = p2_.x;
        aim_point_.y = p2_.y;
        end_point_.x = p1_.x;
        end_point_.y = p1_.y;
    }
}

// 选择y值小的作为起始点 |k| >= 1
void ALGOLine::ChosePointByY() {
    if (p1_.y < p2_.y) {
        aim_point_.x = p1_.x;
        aim_point_.y = p1_.y;
        end_point_.x = p2_.x;
        end_point_.y = p2_.y;
    }
    else {
        aim_point_.x = p2_.x;
        aim_point_.y = p2_.y;
        end_point_.x = p1_.x;
        end_point_.y = p1_.y;
    }
}

void ALGOLine::IncresePointNum() { ++point_num_; }

void ALGOLine::set_p1(unsigned int x, unsigned int y) {
    p1_.x = x;
    p1_.y = y;
}

void ALGOLine::set_p2(unsigned int x, unsigned int y) {
    p2_.x = x;
    p2_.y = y;
}

unsigned int ALGOLine::get_point_num() {
    return point_num_;
}

// 利用斜截式方程 y = kx+b 把b当做0
// y2 = y1+k, x2 = x1+1,  (|k| < 1)
// y2 = y1+1, x2 = x1+1/k (|k| >= 1)  // 说明包含浮点数加法
void ALGOLine::DrawLine_DDA(SDL_Renderer* renderer) {
    if (p2_.x == p1_.x) {  // k = ∞
        ChosePointByY();
        while (aim_point_.y <= end_point_.y) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.y = aim_point_.y + 1;
        }
        return ;
    }

    if (p2_.y == p1_.y) {  // k = 0
        ChosePointByX();
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.x = aim_point_.x + 1;
        }
        return ;
    }

    float k = (float)(p2_.y - p1_.y) / (p2_.x - p1_.x);
    if (abs(k) < 1.0f) {
        ChosePointByX();
        float y = aim_point_.y;
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            ++aim_point_.x;
            y = k + y;
            aim_point_.y = y + 0.5;
        }
    }
    else {
        ChosePointByY();
        float x = aim_point_.x;
        while (aim_point_.y <= end_point_.y) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            ++aim_point_.y;
            x = 1/k + x;
            aim_point_.x = x + 0.5;
        }
    }
}

// 中点画线法
// 利用直线的一般式方程F(x, y) = Ax+By+C
// A=y2-y1, B=x1-x2, C=x2y1-x1y2
// d0=2A+B, d1=d0+2A+2B
// 只需要d的符号
// 注意笛卡尔坐标系y增长方向是往下，所以要把我们平时理解的下方向在这里当作上方向  
void ALGOLine::DrawLine_MidPoint(SDL_Renderer *renderer) {
    if (p2_.x == p1_.x) {  // k = ∞
        ChosePointByY();
        while (aim_point_.y <= end_point_.y) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.y = aim_point_.y + 1;
        }
        return ;
    }

    if (p2_.y == p1_.y) {  // k = 0
        ChosePointByX();
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.x = aim_point_.x + 1;
        }
        return ;
    }

    if (abs(p2_.y-p1_.y) < abs(p2_.x-p1_.x)) {  // |k|<1
        ChosePointByY();  // 选择y坐标小的作为起始点，以x为步进方向，+1或者-1，y要么不变要么+1
        int A = end_point_.y - aim_point_.y;
        int B = aim_point_.x - end_point_.x;
        if (B < 0) {       // 一般式y的系数>0时，计算结果<0点在直线下方
            B = -B;
            A = -A;
        }

        int step = 1;
        if (aim_point_.x > end_point_.x) {  // x步进-1，d的计算方式要改
            step = -1;
        }
        A = step * A;
        int d = A + A + B;

        while (aim_point_.x != end_point_.x) {
            SDL_SetRenderDrawColor(renderer, 0, 255,  0, 255);
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.x += step;
            if (d < 0) {
                ++aim_point_.y;
                d = d + A + A + B + B;
            }
            else {
                d = d + A;
            }
        }
    }
    else {  // y为步进方向，x和y互换，此时 F = (x2-x1)y + (y1-y2)x + C
        ChosePointByX(); 
        int A = end_point_.x - aim_point_.x;
        int B = aim_point_.y - end_point_.y;
        if (B < 0) {
            B = -B;
            A = -A;
        }

        int step = 1;
        if (aim_point_.y > end_point_.y) {
            step = -1;
        }
        A = step * A;
        int d = A + A + B;

        while (aim_point_.y != end_point_.y) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.y += step;
            if (d < 0) {
                ++aim_point_.x;
                d = d + A + A + B + B;
            }
            else {
                d = d + A;
            }
        }
    }
}


