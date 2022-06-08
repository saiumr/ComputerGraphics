#include "ALGOLine.h"
#include "SDL2/SDL_rect.h"

ALGOLine::ALGOLine()  { point_num_ = 0; }

ALGOLine::~ALGOLine() { }

void ALGOLine::_DDA_ChosePointByX() {
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
void ALGOLine::_DDA_ChosePointByY() {
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

// 利用斜截式方程 y = kx+b 把b当做0
// y2 = y1+k, x2 = x1+1,  (|k| < 1)
// y2 = y1+1, x2 = x1+1/k (|k| >= 1)  // 说明包含浮点数加法
void ALGOLine::DDA_DrawLine(SDL_Renderer* renderer) {
    if (p2_.x != p1_.x) {
        float k = (float)(p2_.y - p1_.y) / (p2_.x - p1_.x);

        if (abs(k) < 1.0f) {
            _DDA_ChosePointByX();
            float y = aim_point_.y;
            while (aim_point_.x <= end_point_.x) {
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                ++aim_point_.x;
                y = k + y;
                aim_point_.y = y + 0.5;
            }
        }
        else {
            _DDA_ChosePointByY();
            float x = aim_point_.x;
            while (aim_point_.y <= end_point_.y) {
                SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
                ++aim_point_.y;
                x = 1/k + x;
                aim_point_.x = x + 0.5;
            }
        }
        return ;
    }
    _DDA_ChosePointByY();
    while (aim_point_.y <= end_point_.y) {
        SDL_RenderDrawPoint(renderer, aim_point_.x, aim_point_.y);
        aim_point_.y = aim_point_.y + 1;
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





