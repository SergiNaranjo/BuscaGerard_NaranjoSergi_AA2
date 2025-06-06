#include "Config.h"
#include <fstream>
#include <sstream>

Config::Config(const char* file) : fileName(file)
{
    mapSizeX = mapSizeY = 0;
    cjLife = cjAttack = 0;

    for (int i = 0; i < 2; ++i) tollCost[i] = 0;
    for (int i = 0; i < 3; ++i)
    {
        numPedestrians[i] = 0;
        maxMoneyFromKill[i] = 0;
        pedestrianLife[i] = 0;
        pedestrianAttack[i] = 0;
    }
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
        std::getline(ss, value, ';');
        mapSizeX = std::stoi(value);
        std::getline(ss, value, ';');
        mapSizeY = std::stoi(value);
    }

    if (std::getline(fileStream, line))
    {
        std::stringstream ss(line);
        std::string value;
        std::getline(ss, value, ';');
        cjLife = std::stoi(value);
        std::getline(ss, value, ';');
        cjAttack = std::stoi(value);
    }

    if (std::getline(fileStream, line))
    {
        std::stringstream ss(line);
        std::string value;
        std::getline(ss, value, ';');
        tollCost[0] = std::stoi(value);
        std::getline(ss, value, ';');
        tollCost[1] = std::stoi(value);
    }

    for (int i = 0; i < 3; ++i)
    {
        if (std::getline(fileStream, line))
        {
            std::stringstream ss(line);
            std::string value;

            std::getline(ss, value, ';');
            numPedestrians[i] = std::stoi(value);

            std::getline(ss, value, ';');
            maxMoneyFromKill[i] = std::stoi(value);

            std::getline(ss, value, ';');
            pedestrianLife[i] = std::stoi(value);

            std::getline(ss, value, ';');
            pedestrianAttack[i] = std::stoi(value);
        }
    }

    fileStream.close();
    return true;
}