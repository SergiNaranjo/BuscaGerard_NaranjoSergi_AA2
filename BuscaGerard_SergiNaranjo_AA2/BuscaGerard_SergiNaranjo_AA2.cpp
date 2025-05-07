#include <iostream>
#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>

const int MAPSIZEX = 60;
const int MAPSIZEY = 20;

const int VIEWSIZEX = 21;
const int VIEWSIZEY = 11;

const int NUM_PEDESTRIANS = 3;
const int MAX_MONEY = 10;

class Config
{
private:
    const char* fileName;
    
public:
    int mapSizeX;
    int mapSizeY;

    int numPedestrians[2];
    int tollCost[2];
    int maxMoneyFromKill[2];

    Config(const char* file) : fileName(file)
    {
        mapSizeX = 60;
        mapSizeY = 20;

        numPedestrians[0] = numPedestrians[1] = 0;
        tollCost[0] = tollCost[1] = 0;
        maxMoneyFromKill[0] = maxMoneyFromKill[1] = 0;
    }

    bool Load()
    {
        std::ifstream fileStream(fileName);
        if (!fileStream.is_open())
        {
            return false;
        }
        std::string line;

        if (std::getline(fileStream, line)) {
            std::stringstream ss(line);
            std::string value;
            if (std::getline(ss, value, ';')) mapSizeX = std::stoi(value);
            if (std::getline(ss, value, ';')) mapSizeY = std::stoi(value);
        }

        // Línea 2: peatones;peaje;dineroMax (Los Santos)
        if (std::getline(fileStream, line)) {
            std::stringstream ss(line);
            std::string value;
            if (std::getline(ss, value, ';')) numPedestrians[0] = std::stoi(value);
            if (std::getline(ss, value, ';')) tollCost[0] = std::stoi(value);
            if (std::getline(ss, value, ';')) maxMoneyFromKill[0] = std::stoi(value);
        }

        // Línea 3: peatones;peaje;dineroMax (San Fierro)
        if (std::getline(fileStream, line)) {
            std::stringstream ss(line);
            std::string value;
            if (std::getline(ss, value, ';')) numPedestrians[1] = std::stoi(value);
            if (std::getline(ss, value, ';')) tollCost[1] = std::stoi(value);
            if (std::getline(ss, value, ';')) maxMoneyFromKill[1] = std::stoi(value);
        }

        fileStream.close();
        return true;
    }
};

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    COUNT
};

// ================== MAP ===================
class Map {
private:
    char** grid;
    int width, height;

public:
    Map(int w, int h) : width(w), height(h) {
        grid = new char* [height];
        for (int i = 0; i < height; i++) {
            grid[i] = new char[width];
            for (int j = 0; j < width; j++) {
                grid[i][j] = '.';
            }
        }

        for (int y = 0; y < height; y++) {
            if (y != height / 2) {
                grid[y][width / 3] = 'X';
                grid[y][2 * width / 3] = 'X';
            }
        }
    }

    ~Map() {
        for (int i = 0; i < height; i++) delete[] grid[i];
        delete[] grid;
    }

    char Get(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height)
            return grid[y][x];
        return ' ';
    }

    void Set(int x, int y, char symbol) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            grid[y][x] = symbol;
    }

    void Clear(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            grid[y][x] = '.';
    }

    bool CanPass(int x, int y) const {
        return Get(x, y) == '.' || Get(x, y) == '$';
    }

    void DrawWalls() {
        for (int y = 0; y < height; y++) {
            if (y != height / 2) {
                grid[y][width / 3] = 'X';
                grid[y][2 * width / 3] = 'X';
            }
        }
    }

    void PrintView(int cx, int cy, char symbol, int viewX, int viewY) {
        system("cls");
        for (int dy = -viewY / 2; dy <= viewY / 2; dy++) {
            for (int dx = -viewX / 2; dx <= viewX / 2; dx++) {
                int x = cx + dx;
                int y = cy + dy;
                if (x == cx && y == cy) std::cout << symbol;
                else std::cout << Get(x, y);
            }
            std::cout << std::endl;
        }
    }
};

// ================ PEATÓN ==================
struct Pedestrian {
    int x, y;
    bool alive = true;
    bool horitzontal = true;
    int island;

    void Move(const Map& map, int px, int py) {
        if (!alive) return;

        if (abs(x - px) <= 1 && abs(y - py) <= 1) return;

        int nx = x + (horitzontal ? (rand() % 3 - 1) : 0);
        int ny = y + (!horitzontal ? (rand() % 3 - 1) : 0);

        if (map.CanPass(nx, ny)) {
            x = nx;
            y = ny;
        }
    }
};

// ================ CJ ==================
class CJ {
private:
    int x, y;
    Direction dir;

public:
    int money = 0;

    CJ(int startX, int startY) : x(startX), y(startY), dir(Direction::DOWN) {}

    void Move(Map& map) {
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

    void Attack(Pedestrian* peds, int num, Map& map) {
        for (int i = 0; i < num; i++) {
            if (peds[i].alive && abs(x - peds[i].x) + abs(y - peds[i].y) == 1) {
                peds[i].alive = false;
                map.Set(peds[i].x, peds[i].y, '$');
            }
        }
    }

    void TakeMoney(Map& map, int maxMoney) {
        if (map.Get(x, y) == '$') {
            money += 1 + rand() % maxMoney;
            map.Clear(x, y);
        }
    }

    int GetX() const { return x; }
    int GetY() const { return y; }

    char GetSymbol() const {
        switch (dir) {
        case Direction::UP: return '^';
        case Direction::DOWN: return 'v';
        case Direction::LEFT: return '<';
        case Direction::RIGHT: return '>';
        default: return '?';
        }
    }
};

// ============== MAIN ===================
int main() {
    srand(time(NULL));
    Config config("config.txt");
    if (!config.Load()) return 1;

    Map map(config.mapSizeX, config.mapSizeY);
    CJ cj(5, config.mapSizeY / 2);

    const int totalPeds = config.numPedestrians[0];
    Pedestrian* pedestrians = new Pedestrian[totalPeds];

    for (int i = 0; i < totalPeds; i++) {
        pedestrians[i].x = 1 + rand() % (config.mapSizeX / 3 - 2);
        pedestrians[i].y = 1 + rand() % (config.mapSizeY - 2);
        pedestrians[i].alive = true;
        pedestrians[i].horitzontal = rand() % 2;
        pedestrians[i].island = 0;
    }

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        cj.Move(map);
        cj.TakeMoney(map, config.maxMoneyFromKill[0]);

        if (GetAsyncKeyState(VK_SPACE)) {
            cj.Attack(pedestrians, totalPeds, map);
        }

        for (int i = 0; i < totalPeds; i++) {
            if (!pedestrians[i].alive) continue;
            pedestrians[i].Move(map, cj.GetX(), cj.GetY());
        }

        for (int y = 0; y < config.mapSizeY; y++) {
            for (int x = 0; x < config.mapSizeX; x++) {
                if (map.Get(x, y) != '$') map.Clear(x, y);
            }
        }

        for (int i = 0; i < totalPeds; i++) {
            if (pedestrians[i].alive) map.Set(pedestrians[i].x, pedestrians[i].y, 'P');
        }

        map.DrawWalls();

        map.PrintView(cj.GetX(), cj.GetY(), cj.GetSymbol(), 21, 11);
        std::cout << "Dinero: $" << cj.money << std::endl;
        Sleep(100);
    }

    delete[] pedestrians;
    return 0;
}
