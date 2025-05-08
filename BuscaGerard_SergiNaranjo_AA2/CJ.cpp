#include "CJ.h"

CJ::CJ(int startX, int startY) : x(startX), y(startY), dir(Direction::DOWN) {}

void CJ::Move(Map& map) {
    if (GetAsyncKeyState(VK_UP) & 0x8000 && map.CanPass(x, y - 1)) {
        y--; dir = Direction::UP;
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && map.CanPass(x, y + 1)) {
        y++; dir = Direction::DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && map.CanPass(x - 1, y)) {
        x--; dir = Direction::LEFT;
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && map.CanPass(x + 1, y)) {
        x++; dir = Direction::RIGHT;
    }
}

void CJ::Attack(Pedestrian* peds, int num, Map& map) {
    for (int i = 0; i < num; i++) {
        if (peds[i].alive && abs(x - peds[i].x) + abs(y - peds[i].y) == 1) {
            peds[i].alive = false;
            map.Set(peds[i].x, peds[i].y, '$');
        }
    }
}

void CJ::TakeMoney(Map& map, int maxMoney) {
    if (map.Get(x, y) == '$') {
        money += 1 + rand() % maxMoney;
        map.Clear(x, y);
    }
}

int CJ::GetX() const { return x; }
int CJ::GetY() const { return y; }

char CJ::GetSymbol() const {
    switch (dir) {
    case Direction::UP: return '^';
    case Direction::DOWN: return 'v';
    case Direction::LEFT: return '<';
    case Direction::RIGHT: return '>';
    default: return '?';
    }
}
