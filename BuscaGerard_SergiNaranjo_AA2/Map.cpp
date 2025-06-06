#include "Map.h"
#include <cstdlib>
#include <Windows.h>

Map::Map(int w, int h) : width(w), height(h)
{
    grid = new char* [height];
    for (int i = 0; i < height; i++)
    {
        grid[i] = new char[width];
        for (int j = 0; j < width; j++)
        {
            grid[i][j] = '.';
        }
    }

    for (int y = 0; y < height; y++)
    {
        if (y != height / 2)
        {
            grid[y][width / 3] = 'X';
            grid[y][2 * width / 3] = 'X';
        }
    }
}

Map::~Map()
{
    for (int i = 0; i < height; i++)
        delete[] grid[i];
    delete[] grid;
}

char Map::Get(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        return grid[y][x];
    return ' ';
}

void Map::Set(int x, int y, char symbol)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        grid[y][x] = symbol;
}

void Map::Clear(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        grid[y][x] = '.';
}

bool Map::CanPass(int x, int y) const
{
    char tile = Get(x, y);
    return tile == '.' || tile == '$' || tile == 'T';
}

void Map::DrawWalls()
{
    for (int y = 0; y < height; y++)
    {
        if (y != height / 2)
            grid[y][width / 3] = 'X';
        else
            grid[y][width / 3] = 'T';

        if (y != height / 2)
            grid[y][2 * width / 3] = 'X';
        else
            grid[y][2 * width / 3] = 'T';
    }
}

void Map::PrintView(int cx, int cy, char symbol, int viewX, int viewY)
{
    system("cls");
    for (int dy = -viewY / 2; dy <= viewY / 2; dy++)
    {
        for (int dx = -viewX / 2; dx <= viewX / 2; dx++)
        {
            int x = cx + dx;
            int y = cy + dy;

            if (x == cx && y == cy)
                std::cout << symbol;
            else
                std::cout << Get(x, y);
        }
        std::cout << std::endl;
    }
}
