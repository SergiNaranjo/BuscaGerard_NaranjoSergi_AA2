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

    const int totalCars = 5; // CHANGE TO READ IT FROM CONFIG FILE
    const int totalPeds = config.numPedestrians[0];
    Pedestrian* pedestrians = new Pedestrian[totalPeds];
    Car* cars = new Car[totalCars];

    for (int i = 0; i < totalPeds; i++)
    {
        pedestrians[i].x = 1 + rand() % (config.mapSizeX / 3 - 2);
        pedestrians[i].y = 1 + rand() % (config.mapSizeY - 2);
        pedestrians[i].alive = true;
        pedestrians[i].horitzontal = rand() % 2;
        pedestrians[i].island = 0;
        pedestrians[i].PassiveOrAgressive();
    }

    for (int i = 0; i < totalCars; i++)
    {
        cars[i].x = 1 + rand() % (config.mapSizeX / 3 - 2);
        cars[i].y = 1 + rand() % (config.mapSizeY - 2);
        cars[i].island = 0;
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

        cj.RunOver(pedestrians, totalPeds, map);

        if (GetAsyncKeyState(VK_SPACE))
        {
            cj.EnterCar(cars, totalCars, map);
        }

        cj.GetAttacked(pedestrians, totalPeds);

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

        for (int i = 0; i < totalCars; i++)
        {
            map.Set(cars[i].x, cars[i].y, 'C');
        }
        for (int i = 0; i < totalCars; i++)
        {
            if (!cars[i].entered && cars[i].x >= 0 && cars[i].y >= 0)
            {
                map.Set(cars[i].x, cars[i].y, 'C');
            }
        }


        for (int i = 0; i < totalCars; i++)
        {
            char symbol = 'C';
            map.Set(cars[i].x, cars[i].y, symbol);
        }

        map.DrawWalls();

        map.PrintView(cj.GetX(), cj.GetY(), cj.GetSymbol(), 21, 11);
        std::cout << "Money: $" << cj.money << "  Health: " << cj.GetHealth() << std::endl;
        Sleep(100);
    }

    delete[] pedestrians;
    return 0;
}
