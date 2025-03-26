#ifndef MAP_H //5
#define MAP_H

#include "Game.h"

using namespace std;

class Map{

public:

    Map();
    ~Map();

    static void LoadMap(string path, int sizeX, int sizeY, int gridWidth);

private:

    SDL_Rect src, dest;

    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

};

#endif