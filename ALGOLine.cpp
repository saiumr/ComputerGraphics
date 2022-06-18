#include "ALGOLine.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include <cstdint>
#include <iostream>
#include <iterator>

const uint8_t ALGOLine::kTotalWays= 3;

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
    case kBresenham:
        draw_line_way_ = &ALGOLine::DrawLine_Bresenham;
        break;
    }
}

void ALGOLine::DrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    (this->*draw_line_way_)(renderer, x1, y1, x2, y2);
}

void ALGOLine::EventHandle(SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_s) {
            SwitchWay();
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        ++point_num_;
        if (point_num_ % 2 == 0) {
            end_point_.x = event.motion.x;
            end_point_.y = event.motion.y;
            DrawLine(renderer, aim_point_.x, aim_point_.y, end_point_.x, end_point_.y);
        }
        else {
            aim_point_.x = event.motion.x;
            aim_point_.y = event.motion.y;
        }
    }
}

// 选择x值小的作为起始点 0<=|k|<=1
void ALGOLine::ChosePointByX(const SDL_Point& p1, const SDL_Point& p2) {
    if (p1.x < p2.x) {
        aim_point_.x = p1.x;
        aim_point_.y = p1.y;
        end_point_.x = p2.x;
        end_point_.y = p2.y;
    }
    else {
        aim_point_.x = p2.x;
        aim_point_.y = p2.y;
        end_point_.x = p1.x;
        end_point_.y = p1.y;
    }
}

// 选择y值小的作为起始点 |k| >= 1
void ALGOLine::ChosePointByY(const SDL_Point& p1, const SDL_Point& p2) {
    if (p1.y < p2.y) {
        aim_point_.x = p1.x;
        aim_point_.y = p1.y;
        end_point_.x = p2.x;
        end_point_.y = p2.y;
    }
    else {
        aim_point_.x = p2.x;
        aim_point_.y = p2.y;
        end_point_.x = p1.x;
        end_point_.y = p1.y;
    }
}

// 利用斜截式方程 y = kx+b 把b当做0
// y2 = y1+k, x2 = x1+1,  (|k| < 1)
// y2 = y1+1, x2 = x1+1/k (|k| >= 1)  // 说明包含浮点数加法
void ALGOLine::DrawLine_DDA(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    SDL_Point p1 = {x1, y1};
    SDL_Point p2 = {x2, y2};
    if (p2.x == p1.x) {  // k = ∞
        ChosePointByY(p1, p2);
        while (aim_point_.y <= end_point_.y) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.y = aim_point_.y + 1;
        }
        return ;
    }

    if (p2.y == p1.y) {  // k = 0
        ChosePointByX(p1, p2);
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.x = aim_point_.x + 1;
        }
        return ;
    }

    float k = (float)(p2.y - p1.y) / (p2.x - p1.x);
    if (abs(k) < 1.0f) {
        ChosePointByX(p1, p2);
        float y = aim_point_.y;
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            ++aim_point_.x;
            y = k + y;
            aim_point_.y = y + 0.5;
        }
    }
    else {
        ChosePointByY(p1, p2);
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

// 四个while循环分别掌管四种斜率范围直线的绘制

// 中点画线法
// 利用直线的一般式方程F(x, y) = Ax+By+C
// A=y2-y1, B=x1-x2, C=x2y1-x1y2
// d0=2A+B, d1=d0+2A+2B y=y+1，或者d1=d0+2A y=y
// 只需要d的符号
// 注意笛卡尔坐标系y增长方向是往下，所以要把我们平时理解的下方向在这里当作上方向  
void ALGOLine::DrawLine_MidPoint(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
    SDL_Point p1 = {x1, y1};
    SDL_Point p2 = {x2, y2};
    if (p2.x == p1.x) {  // k不存在
        ChosePointByY(p1, p2);
        while (aim_point_.y <= end_point_.y) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.y = aim_point_.y + 1;
        }
        return ;
    }

    if (p2.y == p1.y) {  // k = 0
        ChosePointByX(p1, p2);
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.x = aim_point_.x + 1;
        }
        return ;
    }

    if (abs(p2.y-p1.y) < abs(p2.x-p1.x)) {  // |k|<1
        ChosePointByY(p1, p2);  // 选择y坐标小的作为起始点，以x为步进方向，+1或者-1，y要么不变要么+1
        int A = aim_point_.y - end_point_.y;
        int B = end_point_.x - aim_point_.x;
        if (B < 0) {       // 一般式y的系数>0时，计算结果<0点在直线下方
            B = -B;
            A = -A;
        }

        if (aim_point_.x < end_point_.x) {
            int d = (A << 1) + B;
            int d1 = (A+B) << 1;  // 2*(A+B)
            int d2 = A << 1;      // 2*A
            while (aim_point_.x <= end_point_.x) {
                SDL_SetRenderDrawColor(renderer, 0, 255,  0, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                ++aim_point_.x;
                if (d < 0) {
                    ++aim_point_.y;
                    d = d + d1;
                }
                else {
                    d = d + d2;
                }
            }
        }
        else {
            int d = -(A<<1) + B;
            int d1 = (B-A) << 1;  // 2*(B-A)
            int d2 = -A << 1;      // -2*A
            while (aim_point_.x >= end_point_.x) {
                SDL_SetRenderDrawColor(renderer, 0, 255,  0, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                --aim_point_.x;
                if (d < 0) {
                    ++aim_point_.y;
                    d = d + d1;
                }
                else {
                    d = d + d2;
                }
            }
        }
    }
    else {  // y为步进方向，x和y互换，此时 F = (x2-x1)y + (y1-y2)x + C
        ChosePointByX(p1, p2); 
        int A = end_point_.x - aim_point_.x;
        int B = aim_point_.y - end_point_.y;
        if (B < 0) {
            B = -B;
            A = -A;
        }

        if (aim_point_.y < end_point_.y) {
            int d = (A << 1) + B;
            int d1 = (A+B) << 1;  // 2*(A+B)
            int d2 = A << 1;      // 2*A
            while (aim_point_.y <= end_point_.y) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                ++aim_point_.y;
                if (d < 0) {
                    ++aim_point_.x;
                    d = d + d1;
                }
                else {
                    d = d + d2;
                }
            }
        }
        else {
            int d = -(A<<1) + B;
            int d1 = (B-A) << 1;  // 2*(B-A)
            int d2 = -A << 1;      // -2*A
            while (aim_point_.y >= end_point_.y) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                --aim_point_.y;
                if (d < 0) {
                    ++aim_point_.x;
                    d = d + d1;
                }
                else {
                    d = d + d2;
                }
            }
        }
    }
}

// Breseham算法，和中点画线的算法差不多的思想
// 但不是拿中点比较，而是直接根据直线上的点到像素格底部的距离大小判断
// 直线更靠近上面还是下面，当然也经过了一系列的优化
// 最主要的是不需要利用任何直线方程就能用的
void ALGOLine::DrawLine_Bresenham(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    SDL_Point p1 = {x1, y1};
    SDL_Point p2 = {x2, y2};
    if (p2.x == p1.x) {  // k不存在
        ChosePointByY(p1, p2);
        while (aim_point_.y <= end_point_.y) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.y = aim_point_.y + 1;
        }
        return ;
    }

    if (p2.y == p1.y) {  // k = 0
        ChosePointByX(p1, p2);
        while (aim_point_.x <= end_point_.x) {
            SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
            aim_point_.x = aim_point_.x + 1;
        }
        return ;
    }
    
    if (abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
        ChosePointByY(p1, p2);
        int dx = abs(end_point_.x - aim_point_.x);
        int dy = abs(end_point_.y - aim_point_.y);
        int e = -dx;
        int e1 = dy<<1;
        int e2 = -dx<<1;

        if (aim_point_.x < end_point_.x) {
            while (aim_point_.x <= end_point_.x) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                ++aim_point_.x;
                e = e + e1;
                if (e > 0) {
                    ++aim_point_.y;
                    e = e + e2;
                }
            }
        }
        else {
            while (aim_point_.x >= end_point_.x) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                --aim_point_.x;
                e = e + e1;
                if (e > 0) {
                    ++aim_point_.y;
                    e = e + e2;
                }
            }
        }
    }
    else {
        ChosePointByX(p1, p2);
        int dx = abs(end_point_.x - aim_point_.x);
        int dy = abs(end_point_.y - aim_point_.y);
        int e = -dx;
        int e1 = -dy<<1;
        int e2 = dx<<1;

        if (aim_point_.y < end_point_.y) {
            while (aim_point_.y <= end_point_.y) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                ++aim_point_.y;
                e = e + e2;
                if (e > 0) {
                    ++aim_point_.x;
                    e = e + e1;
                }
            }
        }
        else {
            while (aim_point_.y >= end_point_.y) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                --aim_point_.y;
                e = e + e2;
                if (e > 0) {
                    ++aim_point_.x;
                    e = e + e1;
                }
            }
        }
    }
}


