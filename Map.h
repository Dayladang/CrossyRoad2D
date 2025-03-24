#ifndef MAP_H //5
#define MAP_H

#include "Game.h"

class Map{

public:

    Map();
    ~Map();

    void LoadMap(int arr[2][1]);
    void DrawMap();

private:

    SDL_Rect src, dest;

    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

    int map[1][1];
};

#endif