#include "Config.h"
#include <fstream>
#include <sstream>
Config::Config(const char* file) : fileName(file)
{
    mapSizeX = 60;
    mapSizeY = 20;

    numPedestrians[0] = numPedestrians[1] = 0;
    tollCost[0] = tollCost[1] = 0;
    maxMoneyFromKill[0] = maxMoneyFromKill[1] = 0;
}

bool Config::Load()
{
    std::ifstream fileStream(fileName);

    if (!fileStream.is_open())
    {
        return false;
    }

    std::string line;

    if (std::getline(fileStream, line)) 
    {
        std::stringstream ss(line);
        std::string value;

        if (std::getline(ss, value, ';'))
        {
            mapSizeX = std::stoi(value);
        }

        if (std::getline(ss, value, ';'))
        {
            mapSizeY = std::stoi(value);
        }
    }

    
    if (std::getline(fileStream, line)) 
    {
        std::stringstream ss(line);
        std::string value;

        if (std::getline(ss, value, ';'))
        {
            numPedestrians[0] = std::stoi(value);
        }

        if (std::getline(ss, value, ';'))
        {
            tollCost[0] = std::stoi(value);
        }

        if (std::getline(ss, value, ';'))
        {
            maxMoneyFromKill[0] = std::stoi(value);
        }
    }

    
    if (std::getline(fileStream, line)) 
    {
        std::stringstream ss(line);
        std::string value;
        if (std::getline(ss, value, ';'))
        {
            numPedestrians[1] = std::stoi(value);
        }

        if (std::getline(ss, value, ';'))
        {
            tollCost[1] = std::stoi(value);
        }

        if (std::getline(ss, value, ';'))
        {
            maxMoneyFromKill[1] = std::stoi(value);
        }
    }

    fileStream.close();
    return true;
}
