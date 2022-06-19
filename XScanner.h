#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "ALGOLine.h"

class XScanner {
private:
    typedef struct Edge {
        int ymax;
        float x;
        float dx;
        struct Edge* next;
    } Edge;

    typedef struct Point {
        int x;
        int y;
    } Point;
    ALGOLine* draw_line_;

    Edge* NET_[600];  // 600 is WindowWidth
    Edge* AET_;
    std::vector<Point> points_;

public:
    XScanner();
    ~XScanner();

    void XScannerAlgorithm(SDL_Renderer* renderer);
    void EventHandle(SDL_Event& event, SDL_Renderer* renderer);

};

