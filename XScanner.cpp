#include "XScanner.h"
#include "ALGOLine.h"
#include "SDL2/SDL_render.h"
#include <iostream>

XScanner::XScanner() {
   draw_line_ = new ALGOLine(); 
}

XScanner::~XScanner() {
    delete draw_line_;
}

void XScanner::XScannerAlgorithm(SDL_Renderer *renderer) {
    //计算最高点的y坐标
    int maxY = 0;
    for (int i = 0; i < points_.size(); i++) {
        if (points_[i].y > maxY) {
            maxY = points_[i].y;
        }
    }
  
    //初始化NET和AET_
    for (int i = 0; i < maxY; i++) {
        NET_[i] = new Edge;
        NET_[i]->next = nullptr;
    }
    AET_ = new Edge;
    AET_->next = nullptr;
  
    // 设置画点颜色为白色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // 建立NET_
    for (int i = 0; i < points_.size(); i++) {
        //取出当前点1前后相邻的共4个点，点1与点2的连线作为本次循环处理的边，另外两个点点0和点3用于计算奇点
        int x0 = points_[(i - 1 + points_.size()) % points_.size()].x;
        int x1 = points_[i].x;
        int x2 = points_[(i + 1) % points_.size()].x;
        int x3 = points_[(i + 2) % points_.size()].x;
        int y0 = points_[(i - 1 + points_.size()) % points_.size()].y;
        int y1 = points_[i].y;
        int y2 = points_[(i + 1) % points_.size()].y;
        int y3 = points_[(i + 2) % points_.size()].y;
        
        //水平线直接舍弃
        if (y1 == y2)
            continue;
        
        //分别计算下端点y坐标、上端点y坐标、下端点x坐标和斜率倒数
        int ymin = y1 > y2 ? y2 : y1;
        int ymax = y1 > y2 ? y1 : y2;
        float x = y1 > y2 ? x2 : x1;
        float dx = (x1 - x2) * 1.0f / (y1 - y2);
    
        //奇点特殊处理，若点2->1->0的y坐标单调递减则y1为奇点，若点1->2->3的y坐标单调递减则y2为奇点
        if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3))) {
            ymin++;
            x += dx;
        }
    
        //创建新边，插入NET_
        Edge *p = new Edge;
        p->ymax = ymax;
        p->x = x;
        p->dx = dx;
        p->next = NET_[ymin]->next;  // 头插法
        NET_[ymin]->next = p;
    }
  
    //扫描线从下往上扫描，y坐标每次加1
    for (int i=0; i<maxY; i++) {
        //取出ET中当前扫描行的所有边并按x的递增顺序（若x相等则按dx的递增顺序）插入AET_
        while (NET_[i]->next) {
            //取出ET中当前扫描行表头位置的边
            Edge *pInsert = NET_[i]->next;
            Edge *p = AET_;
            
            //在AET_中搜索合适的插入位置
            while (p->next) {
                if (pInsert->x > p->next->x) {
                    p = p->next;
                    continue;
                }
                if (pInsert->x == p->next->x && pInsert->dx > p->next->dx) {
                    p = p->next;
                    continue;
                }
            
                //找到位置
                break;
            }
      
            //将pInsert从NET_中删除，并插入AET_的当前位置
            NET_[i]->next = pInsert->next;
            pInsert->next = p->next;
            p->next = pInsert;
        }
    
        //AET_中的边两两配对并填色
        Edge *p = AET_;
        while (p->next && p->next->next) {
            for (int x = p->next->x; x < p->next->next->x; x++) {
                SDL_RenderDrawPoint(renderer, x, i);
            }
            p = p->next->next;
        }
    
        //删除AET_中满足y=ymax的边
        p = AET_;
        while (p->next) {
            if (p->next->ymax == i) {
                Edge *pDelete = p->next;
                p->next = pDelete->next;
                pDelete->next = nullptr;
                delete pDelete;
            }
            else {
                p = p->next;
            }
        }
    
        //更新AET_中边的x值，进入下一循环
        p = AET_;
        while (p->next) {
            p->next->x += p->next->dx;
            p = p->next;
        }
    }
}

void XScanner::EventHandle(SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            Point p = {event.motion.x, event.motion.y};
            points_.push_back(p);
            if (points_.size() > 1) {
                draw_line_->DrawLine(renderer, points_[points_.size()-1].x, points_[points_.size()-1].y, points_[points_.size()-2].x, points_[points_.size()-2].y);
            }
        }
        
        // finish polygon
        if (points_.size()>2 && event.button.button == SDL_BUTTON_RIGHT) {
            draw_line_->DrawLine(renderer, points_[points_.size()-1].x, points_[points_.size()-1].y, points_[0].x, points_[0].y);
            //********** XSCANNER ALGORITHM ***********//
            XScannerAlgorithm(renderer);
            points_.clear();
        }
    }
}


