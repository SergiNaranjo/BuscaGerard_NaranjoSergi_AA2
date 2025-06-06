#include "Main_Menu_and_GameOver.h"
#include <iostream>

void MainMenuAndGameOver::Menu()
{
    while (started != 0 && started != 1)
    {
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
