#include "Config.h"
#include "Map.h"
#include "Pedestrian.h"
#include "CJ.h"
#include "Util.h"
#include "Splash Screen.h"
#include "Main_Menu_and_GameOver.h"

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
    SplashScreen splashScreen;
    MainMenuAndGameOver mainMenu;

    const int totalCars = 5; // CHANGE TO READ IT FROM CONFIG FILE
    const int totalPeds = config.numPedestrians[0];
    Pedestrian* pedestrians = new Pedestrian[totalPeds];
    Car* cars = new Car[totalCars];

    for (int i = 0; i < totalPeds; i++)
    {
        int thirdIslandXStart = 2 * config.mapSizeX / 3;
        pedestrians[totalPeds - 1].x = thirdIslandXStart + rand() % (config.mapSizeX - thirdIslandXStart - 1);
        pedestrians[totalPeds - 1].y = 1 + rand() % (config.mapSizeY - 2);
        pedestrians[totalPeds - 1].island = 2;
        pedestrians[i].PassiveOrAgressive();
    }
    int perIsland = totalPeds / 3;
    int remainder = totalPeds % 3;

    int islandOffsets[3] = {
        0,
        config.mapSizeX / 3,
        2 * config.mapSizeX / 3
    };

    for (int i = 0; i < totalPeds; i++)
    {
        int island = i / perIsland;
        if (island > 2) island = 2;

        pedestrians[i].island = island;

        int xStart = islandOffsets[island];
        int xEnd = (island == 2) ? config.mapSizeX - 1 : islandOffsets[island + 1] - 1;

        pedestrians[i].x = xStart + 1 + rand() % (xEnd - xStart - 1);
        pedestrians[i].y = 1 + rand() % (config.mapSizeY - 2);
        pedestrians[i].alive = true;
        pedestrians[i].horitzontal = rand() % 2;
        pedestrians[i].PassiveOrAgressive();

        
        switch (island)
        {
        case 0: pedestrians[i].hits = 1; break; // Read from file
        case 1: pedestrians[i].hits = 2; break;
        case 2: pedestrians[i].hits = 3; break;
        }
    }
    pedestrians[totalPeds -1].InmuneToRunOver();
    pedestrians[totalPeds - 1].hits = 5;
    pedestrians[totalPeds - 1].SetAgressive();

    for (int i = 0; i < totalCars; i++)
    {
        cars[i].x = 1 + rand() % (config.mapSizeX / 3 - 2);
        cars[i].y = 1 + rand() % (config.mapSizeY - 2);
        cars[i].island = 0;
    }
    int carsPerIsland = totalCars / 3;
    int extra = totalCars % 3;

    int carIndex = 0;
    for (int island = 0; island < 3; island++)
    {
        int carsInThisIsland = carsPerIsland + (island < extra ? 1 : 0);
        int xStart = island * config.mapSizeX / 3;
        int xEnd = (island == 2) ? config.mapSizeX - 1 : (island + 1) * config.mapSizeX / 3 - 1;

        for (int j = 0; j < carsInThisIsland && carIndex < totalCars; j++, carIndex++)
        {
            cars[carIndex].x = xStart + 1 + rand() % (xEnd - xStart - 2);
            cars[carIndex].y = 1 + rand() % (config.mapSizeY - 2);
            cars[carIndex].island = island;
        }
    }

    splashScreen.PrintSplashScreen();
    mainMenu.Menu();
    
    if(mainMenu.GetStarted() == 0)
    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            break;
        }

        cj.Move(map, mainMenu);
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

        cj.GetAttacked(pedestrians, totalPeds, mainMenu);

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
            if (pedestrians[i].alive && !pedestrians[i].GetInmuneToRunOver()) map.Set(pedestrians[i].x, pedestrians[i].y, 'P');
            else if (pedestrians[i].alive && pedestrians[i].GetInmuneToRunOver()) map.Set(pedestrians[i].x, pedestrians[i].y, 'B');
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
        if (cj.GetHealth() == 0)
            mainMenu.SetStared(3);

        if (pedestrians[totalPeds - 1].hits == 0)
        {
            system("cls");
            mainMenu.GameOver();
            exit(0);
        }

        Sleep(100);
    }
    
    delete[] pedestrians;
    return 0;
}
