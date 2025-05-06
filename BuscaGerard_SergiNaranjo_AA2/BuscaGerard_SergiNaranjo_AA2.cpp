#include <iostream>
#define InitialPosition 5
#define MapSizeX 10
#define MapSizeY 10
class CJ
{
public:
    CJ();
    ~CJ();

    void addMoney(int addingAmount)
    {
        money += addingAmount;
    }
    void MoveRight()
    {
        posX += 1;
    }
    void MoveLeft()
    {
        posX -= 1;
    }
    void MoveUp()
    {
        posY += 1;
    }
    void MoveDown()
    {
        posY -= 1;
    }
private:
    int posX;
    int posY;
    int money;
};

CJ::CJ()
{
    posX = InitialPosition;
    posY = InitialPosition;
    money = 0;
}

CJ::~CJ()
{
}

class Map
{
public:
    

private:
    Map()
    {
        char map[MapSizeX][MapSizeY];

        for (int i = 0; i < MapSizeX; i++)
        {
            for (int j = 0; j < MapSizeY; j++)
            {
                map[i][j] = 0;
            }
        }
    }
};

int main()
{

}
