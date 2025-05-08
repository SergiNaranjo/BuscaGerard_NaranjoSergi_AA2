#pragma once
#include "Map.h"
#include "Pedestrian.h"
#include <Windows.h>

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    COUNT
};


class CJ 
{
private:
    int x, y;
    Direction dir;

public:
    int money = 0;

    CJ(int startX, int startY);

    void Move(Map& map);

    void Attack(Pedestrian* peds, int num, Map& map);

    void TakeMoney(Map& map, int maxMoney);

    int GetX() const;
    int GetY() const;

    char GetSymbol() const;
};

