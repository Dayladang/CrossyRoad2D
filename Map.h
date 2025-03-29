#ifndef MAP_H //5
#define MAP_H

#include "Game.h"

using namespace std;

class Map{

public:

    Map(const char* maplink, int mapscale, int tilesize);
    ~Map();

    void LoadMap(string path, int sizeX, int sizeY, int gridWidth);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    const char* mapLink;
    int mapScale;
    int tileSize;

};

#endif