#pragma once
#include "Map.h"
#include <cstdlib>
#include <ctime>

struct Pedestrian
{
    int hits = 2;
    int x, y;
    bool alive = true;
    bool horitzontal = true;
    int island;
    bool passive;
    bool attackingCJ = false;
    bool inmuneRunOver = false;
    clock_t lastAttackTime = 0;

    void Move(const Map& map, int px, int py);

    void PassiveOrAgressive();
    void SetAgressive();
    void InmuneToRunOver();
    bool GetInmuneToRunOver();

    void Hurt();
};