#include <iostream>
#include <ctime>
#include <cstdlib>
#include <Windows.h>

const int MAPSIZEX = 60;
const int MAPSIZEY = 20;

const int VIEWSIZEX = 21;
const int VIEWSIZEY = 11;

const int NUM_PEDESTRIANS = 3;
const int MAX_MONEY = 10;

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    COUNT
};

// MAPA
class Map
{
private:
    char** grid;

public:
    Map()
    {
        grid = new char* [MAPSIZEY];

        for (int i = 0; i < MAPSIZEY; i++)
        {
            grid[i] = new char[MAPSIZEX];

            for (int j = 0; j < MAPSIZEX; j++)
            {
                grid[i][j] = '.';
            }
        }

        for (int y = 0; y < MAPSIZEY; y++)
        {
            if (y != MAPSIZEY / 2)
            {
                grid[y][MAPSIZEX / 3] = 'X';
                grid[y][2 * MAPSIZEX / 3] = 'X';
            }
        }
    }

    ~Map()
    {
        for (int i = 0; i < MAPSIZEY; i++)
        {
            delete[] grid[i];
        }
        delete[] grid;
    }

    char Get(int x, int y) const
    {
        if (x >= 0 and x < MAPSIZEX and y >= 0 and y < MAPSIZEY)
        {
            return grid[y][x];
        }

        return ' ';
    }

    char GetSimbol(int x, int y) const
    {
        return Get(x, y);
    }

    void Set(int x, int y, char simbol)
    {
        if (x >= 0 and x < MAPSIZEX and y >= 0 and y < MAPSIZEY)
        {
            grid[y][x] = simbol;
        }
    }

    void Clear(int x, int y)
    {
        if (x >= 0 and x < MAPSIZEX and y >= 0 and y < MAPSIZEY)
        {
            grid[y][x] = '.';
        }
    }

    bool CanPass(int x, int y)
    {
        char c = Get(x, y);
        return c == '.' or c == '$';
    }

    void PrintView(int cx, int cy, char simbolCJ)
    {
        system("cls");

        for (int dy = -VIEWSIZEY / 2; dy <= VIEWSIZEY / 2; dy++)
        {
            for (int dx = -VIEWSIZEX / 2; dx <= VIEWSIZEX / 2; dx++)
            {
                int x = cx + dx;
                int y = cy + dy;

                if (x == cx and y == cy)
                {
                    std::cout << simbolCJ;
                }
                else
                {
                    std::cout << Get(x, y);
                }
            }
            std::cout << std::endl;
        }
    }
};

// PEATONES
struct Pedestrian
{
    int x;
    int y;
    bool alive = true;
    bool horitzontal;
    int island;

    void Move(const Map& map, int px, int py)
    {
        if (!alive)
        {
            return;
        }

        if (std::abs(x - px) <= 1 and std::abs(y - py) <= 1)
        {
            return;
        }

        int nx = x + (horitzontal ? (rand() % 3 - 1) : 0);
        int ny = y + (!horitzontal ? (rand() % 3 - 1) : 0);

        if (map.GetSimbol(nx, ny) == '.') {
            x = nx;
            y = ny;
        }
    }
};


// MOVIMIENTO DE CJ
class CJ
{
private:
    int x;
    int y;
    Direction direction;

public:
    int money = 0;

    CJ(int startX, int startY) : x(startX), y(startY), direction(Direction::DOWN) {}

    void Move(Map& map)
    {
        if (GetAsyncKeyState(VK_UP)) 
        {
            if (map.CanPass(x, y - 1))
            {
                y--;
            }

            direction = Direction::UP;
        }
        else if (GetAsyncKeyState(VK_DOWN)) 
        {
            if (map.CanPass(x, y + 1))
            {
                y++;
            }
            direction = Direction::DOWN;
        }
        else if (GetAsyncKeyState(VK_LEFT))
        {
            if (map.CanPass(x - 1, y))
            {
                x--;
            }

            direction = Direction::LEFT;
        }
        else if (GetAsyncKeyState(VK_RIGHT))
        {
            if (map.CanPass(x + 1, y))
            {
                x++;
            }
            direction = Direction::RIGHT;
        }
    }

    int GetX() const
    {
        return x;
    }

    int GetY() const
    {
        return y;
    }

    char GetSimbol() const
    {
        switch (direction)
        {
        case Direction::UP:
            return '^';
        case Direction::DOWN:
            return 'v';
        case Direction::LEFT:
            return '<';
        case Direction::RIGHT:
            return '>';
        }

        return '?';
    }

    void Attack(Pedestrian* pedestrian, Map& map)
    {
        for (int i = 0; i < NUM_PEDESTRIANS; i++)
        {
            if (pedestrian[i].alive and abs(x - pedestrian[i].x) + abs(y - pedestrian[i].y) == 1)
            {
                pedestrian[i].alive = false;
                map.Set(pedestrian[i].x, pedestrian[i].y, '$');
            }
        }
    }

    void TakeMoney(Map& map)
    {
        if (map.Get(x, y) == '$')
        {
            money += 1 + rand() % MAX_MONEY;
            map.Clear(x, y);
        }
    }
};

void GeneratePedestrian(Pedestrian& p, int island)
{
    int startX = island * MAPSIZEX / 3 + 1;
    int endX = (island + 1) * MAPSIZEX / 3 - 2;

    p.x = startX + rand() % (endX - startX);
    p.y = 1 + rand() % (MAPSIZEY - 2);
    p.alive = true;
    p.horitzontal = rand() % 2;
    p.island = island;
}

int main()
{
    srand(time(NULL));
    Map map;
    CJ cj(5, MAPSIZEY / 2);
    Pedestrian pedestrian[NUM_PEDESTRIANS];

    for (int i = 0; i < NUM_PEDESTRIANS; i++)
    {
        GeneratePedestrian(pedestrian[i], i);
    }

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE)) 
        {
            break;
        }

        cj.Move(map);
        cj.TakeMoney(map);

        if (GetAsyncKeyState(VK_SPACE))
        {
            cj.Attack(pedestrian, map);
        }

        for (int i = 0; i < NUM_PEDESTRIANS; i++)
        {
            if (!pedestrian[i].alive)
            {
                GeneratePedestrian(pedestrian[i], pedestrian[i].island);
            }

            pedestrian[i].Move(map, cj.GetX(), cj.GetY());
        }

        for (int y = 0; y < MAPSIZEY; y++)
        {
            for (int x = 0; x < MAPSIZEX; x++)
            {
                char c = map.Get(x, y);

                // Borra solo si es un peatón 'P' (o cualquier otro símbolo temporal)
                if (c == 'P')
                {
                    map.Clear(x, y);
                }
            }
        }

        for (int i = 0; i < NUM_PEDESTRIANS; i++)
        {
            if (pedestrian[i].alive)
            {
                map.Set(pedestrian[i].x, pedestrian[i].y, 'P');
            }
        }

        map.PrintView(cj.GetX(), cj.GetY(), cj.GetSimbol());
        std::cout << "Money: $" << cj.money << std::endl;
        Sleep(100);
    }
}
