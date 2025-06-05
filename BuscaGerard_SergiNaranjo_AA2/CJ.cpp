#include "CJ.h"
#include "Pedestrian.h"

CJ::CJ(int startX, int startY) : x(startX), y(startY), dir(Direction::DOWN) {}

void CJ::Move(Map& map)
{

    if (GetAsyncKeyState(VK_UP) & 0x8000 && map.CanPass(x, y - 1))
    {
        y--; dir = Direction::UP;
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && map.CanPass(x, y + 1))
    {
        y++; dir = Direction::DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && map.CanPass(x - 1, y))
    {
        x--; dir = Direction::LEFT;
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && map.CanPass(x + 1, y))
    {
        x++; dir = Direction::RIGHT;
    }
}

void CJ::Attack(Pedestrian* peds, int num, Map& map)
{
    for (int i = 0; i < num; i++)
    {
        if (peds[i].alive && abs(x - peds[i].x) + abs(y - peds[i].y) == 1)
        {
            peds[i].Hurt();

            // Si murió tras recibir el golpe, poner $
            if (!peds[i].alive)
            {
                map.Set(peds[i].x, peds[i].y, '$');
            }
            if (!peds[i].passive) {
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
        if (currentCar == nullptr) return;

        // Buscar casilla libre adyacente al coche
        const int cx = x;
        const int cy = y;

        const int offsets[4][2] = {
            {0, -1}, // arriba
            {0, 1},  // abajo
            {-1, 0}, // izquierda
            {1, 0}   // derecha
        };

        for (int i = 0; i < 4; ++i)
        {
            int nx = cx + offsets[i][0];
            int ny = cy + offsets[i][1];

            if (map.CanPass(nx, ny))
            {
                // CJ se baja
                x = nx;
                y = ny;

                // Coche reaparece en su antigua posición
                currentCar->x = cx;
                currentCar->y = cy;
                currentCar->entered = true;

                // CJ ya no está en coche
                transformedIntoCar = false;
                currentCar = nullptr;
                return;
            }
        }

        // No hay espacio para bajarse → no hacer nada
        return;
    }

    // Entrar al coche si hay uno adyacente y sin usar
    for (int i = 0; i < totalCars; ++i)
    {
        if (cars[i].entered) continue;

        int dx = abs(x - cars[i].x);
        int dy = abs(y - cars[i].y);

        if (dx + dy == 1)
        {
            // Subirse al coche
            x = cars[i].x;
            y = cars[i].y;

            transformedIntoCar = true;
            currentCar = &cars[i];

            // Ocultar coche temporalmente
            cars[i].x = -1;
            cars[i].y = -1;
            return;
        }
    }
}

void CJ::GetAttacked(Pedestrian* peds, int num) {
    for (int i = 0; i < num; i++) {
        if (!peds[i].alive || peds[i].passive || !peds[i].attackingCJ)
            continue;

        if (abs(x - peds[i].x) + abs(y - peds[i].y) == 1) {
            clock_t now = clock();
            double secondsSinceLastAttack = (double)(now - peds[i].lastAttackTime) / CLOCKS_PER_SEC;

            if (secondsSinceLastAttack >= 1.0) {
                ReceiveDamage(10); // Daño fijo por ahora
                peds[i].lastAttackTime = now;
            }
        }
    }
}

void CJ::ReceiveDamage(int amountOfDamage) {
    health -= amountOfDamage;

    std::cout << "CJ recibió " << amountOfDamage << " de daño. Salud: " << health << "\n";

    if (health <= 0) {
        std::cout << "CJ ha muerto.\n";
        // exit(0); // opcional
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
        if (peds[i].alive && abs(x - peds[i].x) + abs(y - peds[i].y) == 1 && transformedIntoCar)
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
