#include "AssetManager.h"

using namespace std;

AssetManager::AssetManager(Manager* man){
    manager = man;
}

AssetManager::~AssetManager(){}

void AssetManager::AddTexture(string id, const char* path){
    textures.emplace(id , IMG_LoadTexture(Game::renderer, path) ); // add to textures map
}

SDL_Texture* AssetManager::GetTexture(string id){
    return textures[id];
}