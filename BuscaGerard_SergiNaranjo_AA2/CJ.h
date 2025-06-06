#pragma once
#include "Map.h"
#include "Pedestrian.h"
#include "Car.h"
#include "Main_Menu_and_GameOver.h"
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
    bool paidLeftBorder = false;
    bool paidRightBorder = false;
    const int tollCost = 10;  // Costo del peaje (ajustable)


    clock_t lastTimeHit = 0;

public:
    int money = 0;

    CJ(int startX, int startY);

    void Move(Map& map, MainMenuAndGameOver gameOver);

    void Attack(Pedestrian* peds, int num, Map& map);

    void TakeMoney(Map& map, int maxMoney);

    void EnterCar(Car* car, int num, Map& map);

    void RunOver(Pedestrian* peds, int num, Map& map);

    void ReceiveDamage(int amountOfDamage, MainMenuAndGameOver gameOver);

    void GetAttacked(Pedestrian* peds, int num, MainMenuAndGameOver gameOver);

    int GetHealth();

    int GetX() const;
    int GetY() const;

    char GetSymbol() const;
};