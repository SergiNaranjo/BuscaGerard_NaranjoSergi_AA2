#include "Config.h"
#include "Map.h"
#include "Pedestrian.h"
#include "CJ.h"
#include "Util.h"

#include <ctime>
#include <Windows.h>
#include <cstdlib>
#include <iostream>


int main() 
{
    srand(time(NULL));
    Config config("config.txt");
    if (!config.Load()) return 1;

    Map map(config.mapSizeX, config.mapSizeY);
    CJ cj(5, config.mapSizeY / 2);

    const int totalPeds = config.numPedestrians[0];
    Pedestrian* pedestrians = new Pedestrian[totalPeds];

    for (int i = 0; i < totalPeds; i++) 
    {
        pedestrians[i].x = 1 + rand() % (config.mapSizeX / 3 - 2);
        pedestrians[i].y = 1 + rand() % (config.mapSizeY - 2);
        pedestrians[i].alive = true;
        pedestrians[i].horitzontal = rand() % 2;
        pedestrians[i].island = 0;
    }

    while (true) 
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            break;
        }

        cj.Move(map);
        cj.TakeMoney(map, config.maxMoneyFromKill[0]);

        if (GetAsyncKeyState(VK_SPACE)) 
        {
            cj.Attack(pedestrians, totalPeds, map);
        }

        for (int i = 0; i < totalPeds; i++) 
        {
            if (!pedestrians[i].alive) continue;
            pedestrians[i].Move(map, cj.GetX(), cj.GetY());
        }

        for (int y = 0; y < config.mapSizeY; y++) 
        {
            for (int x = 0; x < config.mapSizeX; x++) 
            {
                if (map.Get(x, y) != '$') map.Clear(x, y);
            }
        }

        for (int i = 0; i < totalPeds; i++) 
        {
            if (pedestrians[i].alive) map.Set(pedestrians[i].x, pedestrians[i].y, 'P');
        }

        map.DrawWalls();

        map.PrintView(cj.GetX(), cj.GetY(), cj.GetSymbol(), 21, 11);
        std::cout << "Money: $" << cj.money << std::endl;
        Sleep(100);
    }

    delete[] pedestrians;
    return 0;
}
