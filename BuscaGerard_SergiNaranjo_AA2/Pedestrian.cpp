#include "Pedestrian.h"

void Pedestrian::Move(const Map& map, int px, int py)
{
    if (!alive) return;

    if (abs(x - px) <= 1 && abs(y - py) <= 1) return;

    int nx = x + (horitzontal ? (rand() % 3 - 1) : 0);
    int ny = y + (!horitzontal ? (rand() % 3 - 1) : 0);

    if (map.CanPass(nx, ny))
    {
        x = nx;
        y = ny;
    }
}

void Pedestrian::PassiveOrAgressive()
{
    passive = rand() % 2;
}

void Pedestrian::Hurt()
{
    hits--;
    if (hits <= 0)
        alive = false;
}

void Pedestrian::SetAgressive()
{
    passive = false;
}

void Pedestrian::InmuneToRunOver()
{
    inmuneRunOver = true;
}
bool Pedestrian::GetInmuneToRunOver()
{
    return inmuneRunOver;
}