#include "Main_Menu_and_GameOver.h"
#include <iostream>
#include <Windows.h>

void MainMenuAndGameOver::Menu()
{
    while (started != 0 && started != 1)
    {
        system("cls");
        std::cout << "*****************************************" << std::endl;
        std::cout << "*               MAIN MENU               *" << std::endl;
        std::cout << "*                                       *" << std::endl;
        std::cout << "*                Start(0)               *" << std::endl;
        std::cout << "*                Exit(1)                *" << std::endl;
        std::cout << "*****************************************" << std::endl;
        std::cout << "*                                       *" << std::endl;
        std::cout << "*                                       *" << std::endl;
        std::cout << "INSERT OPTION: ";
        std::cin >> started;
    }   
}

int MainMenuAndGameOver::GetStarted()
{
    return started;
}

void MainMenuAndGameOver::SetStared(int startedValue)
{
    started = startedValue;
}

void MainMenuAndGameOver::GameOver()
{
    std::cout << "*****************************************" << std::endl;
    std::cout << "*                                       *" << std::endl;
    std::cout << "*                                       *" << std::endl;
    std::cout << "*               GAME OVER!              *" << std::endl;
    std::cout << "*                                       *" << std::endl;
    std::cout << "*                                       *" << std::endl;
    std::cout << "*****************************************" << std::endl;

}
