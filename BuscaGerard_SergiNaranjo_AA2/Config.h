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

    int tollCost[2];

    int numPedestrians[3];
    int maxMoneyFromKill[3];
    int pedestrianLife[3];
    int pedestrianAttack[3];

    Config(const char* file);

    bool Load();
};
