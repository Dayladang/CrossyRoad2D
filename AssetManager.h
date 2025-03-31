#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "ECS/ECS.h"
#include "Vector2D.h"
#include <string>
#include <map>

using namespace std;

class AssetManager {

private:
    map< string , SDL_Texture* > textures; // vector lưu các texture
    Manager* manager; //

public:

    AssetManager(Manager* man);

    ~AssetManager();

    void AddTexture(string id, const char* path);

    SDL_Texture* GetTexture(string id);
};

#endif