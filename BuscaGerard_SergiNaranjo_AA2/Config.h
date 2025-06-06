#pragma once
#include <string>

class Config
{
private:
    const char* fileName;

public:
    int mapSizeX;
    int mapSizeY;

    int cjLife;
    int cjAttack;

    int tollCost[2]; // [0] = San Fierro, [1] = Las Venturas

    int numPedestrians[3];        // 0: LS, 1: SF, 2: LV
    int maxMoneyFromKill[3];
    int pedestrianLife[3];
    int pedestrianAttack[3];

    Config(const char* file);

    bool Load();
};
