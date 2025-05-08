#pragma once
#include "Map.h"

struct Pedestrian {
    int x, y;
    bool alive = true;
    bool horitzontal = true;
    int island;

    void Move(const Map& map, int px, int py);
};

