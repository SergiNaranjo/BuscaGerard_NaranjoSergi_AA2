#include "CJ.h"
#include "Pedestrian.h"
#include "Map.h"
#include "Main_Menu_and_GameOver.h"

CJ::CJ(int startX, int startY, int hp, int atk, int tollLeft, int tollRight) : x(startX), y(startY), dir(Direction::DOWN),
health(hp), attack(atk), tollCostLeft(tollLeft), tollCostRight(tollRight) {
}

void CJ::Move(Map& map, MainMenuAndGameOver gameOver)
{
    int nextX = x, nextY = y;

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        nextY--; dir = Direction::UP;
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        nextY++; dir = Direction::DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        nextX--; dir = Direction::LEFT;
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        nextX++; dir = Direction::RIGHT;
    }

    if (!map.CanPass(nextX, nextY))
    {
        return;
    }

    int mapW = map.GetWidth();
    if ((x == mapW / 3 - 1 && nextX == mapW / 3) && !paidLeftBorder)
    {
        if (money >= tollCostLeft)
        {
            money -= tollCostLeft;
            paidLeftBorder = true;
        }
        else
        {
            system("cls");
            gameOver.GameOver();
            Sleep(5000);
            exit(0);
        }
    }
    else if ((x == 2 * mapW / 3 - 1 && nextX == 2 * mapW / 3) && !paidRightBorder)
    {
        if (money >= tollCostRight)
        {
            money -= tollCostRight;
            paidRightBorder = true;
        }
        else
        {
            system("cls");
            gameOver.GameOver();
            Sleep(5000);
            exit(0);
        }
    }

    x = nextX;
    y = nextY;
}

void CJ::Attack(Pedestrian* peds, int num, Map& map)
{
    for (int i = 0; i < num; i++)
    {
        if (peds[i].alive && abs(x - peds[i].x) + abs(y - peds[i].y) == 1)
        {
            peds[i].Hurt();

            if (!peds[i].alive)
            {
                map.Set(peds[i].x, peds[i].y, '$');
            }
            if (!peds[i].passive)
            {
                peds[i].attackingCJ = true;
                peds[i].lastAttackTime = clock();
            }
        }
    }
}

void CJ::EnterCar(Car* cars, int totalCars, Map& map)
{
    if (transformedIntoCar)
    {
        if (currentCar == nullptr)
        {
            return;
        }

        const int cx = x;
        const int cy = y;

        const int offsets[4][2] =
        {
            {0, -1}, {0, 1}, {-1, 0}, {1, 0}
        };

        for (int i = 0; i < 4; ++i)
        {
            int nx = cx + offsets[i][0];
            int ny = cy + offsets[i][1];

            if (map.CanPass(nx, ny))
            {
                x = nx;
                y = ny;
                currentCar->x = cx;
                currentCar->y = cy;
                currentCar->entered = true;
                transformedIntoCar = false;
                currentCar = nullptr;
                return;
            }
        }
        return;
    }

    for (int i = 0; i < totalCars; ++i)
    {
        if (cars[i].entered)
        {
            continue;
        }

        int dx = abs(x - cars[i].x);
        int dy = abs(y - cars[i].y);

        if (dx + dy == 1)
        {
            x = cars[i].x;
            y = cars[i].y;
            transformedIntoCar = true;
            currentCar = &cars[i];
            cars[i].x = -1;
            cars[i].y = -1;
            return;
        }
    }
}

void CJ::GetAttacked(Pedestrian* peds, int num, MainMenuAndGameOver gameOver)
{
    for (int i = 0; i < num; i++)
    {
        if (!peds[i].alive || peds[i].passive || !peds[i].attackingCJ)
        {
            continue;
        }

        if (abs(x - peds[i].x) + abs(y - peds[i].y) == 1)
        {
            clock_t now = clock();
            double secondsSinceLastAttack = (double)(now - peds[i].lastAttackTime) / CLOCKS_PER_SEC;

            if (secondsSinceLastAttack >= 1.0)
            {
                ReceiveDamage(10, gameOver);
                peds[i].lastAttackTime = now;
            }
        }
    }
}

void CJ::ReceiveDamage(int amountOfDamage, MainMenuAndGameOver gameOver)
{
    health -= amountOfDamage;

    if (health <= 0)
    {
        gameOver.GameOver();
        Sleep(5000);
        exit(0);
    }
}

void CJ::TakeMoney(Map& map, int maxMoney)
{
    if (map.Get(x, y) == '$' && !transformedIntoCar)
    {
        money += 1 + rand() % maxMoney;
        map.Clear(x, y);
    }
}

void CJ::RunOver(Pedestrian* peds, int num, Map& map)
{
    for (int i = 0; i < num; i++)
    {
        if (peds[i].alive && abs(x - peds[i].x) + abs(y - peds[i].y) == 1 && transformedIntoCar && !peds[i].GetInmuneToRunOver())
        {
            peds[i].alive = false;
            map.Set(peds[i].x, peds[i].y, '$');
        }
    }
}

int CJ::GetHealth()
{
    return health;
}

int CJ::GetX() const
{
    return x;
}

int CJ::GetY() const
{
    return y;
}

char CJ::GetSymbol() const
{
    if (transformedIntoCar)
        return 'C';

    switch (dir)
    {
    case Direction::UP:
        return '^';
    case Direction::DOWN:
        return 'v';
    case Direction::LEFT:
        return '<';
    case Direction::RIGHT:
        return '>';
    default:
        return '?';
    }
}