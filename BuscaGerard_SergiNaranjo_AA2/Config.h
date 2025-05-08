#pragma once
#include <string>

class Config
{
private:
    const char* fileName;

public:
    int mapSizeX;
    int mapSizeY;

    int numPedestrians[2];
    int tollCost[2];
    int maxMoneyFromKill[2];

    Config(const char* file);

    bool Load();
   
};

