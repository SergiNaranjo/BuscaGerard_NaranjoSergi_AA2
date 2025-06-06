#pragma once
#include<iostream>

class Map
{
private:
    char** grid;
    int width, height;


public:
    Map(int w, int h);

    ~Map();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    char Get(int x, int y) const;

    void Set(int x, int y, char symbol);

    void Clear(int x, int y);

    bool CanPass(int x, int y) const;

    void DrawWalls();

    void PrintView(int cx, int cy, char symbol, int viewX, int viewY);

};