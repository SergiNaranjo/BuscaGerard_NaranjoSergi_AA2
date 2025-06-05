#pragma once
#include "Map.h"
#include "Pedestrian.h"
#include "Car.h"
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
    bool isInCar = false;
    Car* currentCar = nullptr;
    bool transformedIntoCar = false;
    int health = 100;


    clock_t lastTimeHit = 0;

public:
    int money = 0;

    CJ(int startX, int startY);

    void Move(Map& map);

    void Attack(Pedestrian* peds, int num, Map& map);

    void TakeMoney(Map& map, int maxMoney);

    void EnterCar(Car* car, int num, Map& map);

    void RunOver(Pedestrian* peds, int num, Map& map);

    void ReceiveDamage(int amountOfDamage);

    void GetAttacked(Pedestrian* peds, int num);

    int GetHealth();

    int GetX() const;
    int GetY() const;

    char GetSymbol() const;
};