#ifndef MAP_H //5
#define MAP_H

#include <string>

using namespace std;

class Map{

public:

    Map(string tID, int mapscale, int tilesize);
    ~Map();

    void LoadMap(string path, int sizeX, int sizeY, int gridWidth, int offsetY);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    string texID;
    int mapScale;
    int tileSize;
    int mapOffsetY;

};

#endif