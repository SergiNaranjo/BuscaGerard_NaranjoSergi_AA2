#include "Splash Screen.h"

void SplashScreen::PrintSplashScreen()
{
    std::cout << "*****************************************" << std::endl;
    std::cout << "*      GRAND THEFT AUTO: ENTI CITY      *" << std::endl;
    std::cout << "*              NOW LOADING...           *" << std::endl;
    std::cout << "*****************************************" << std::endl;

    // Esperar 3 segundos
    std::this_thread::sleep_for(std::chrono::seconds(3));

}
