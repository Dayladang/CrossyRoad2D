#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "ECS/ECS.h"
#include "Vector2D.h"
#include <string>
#include <map>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class AssetManager {

private:
    Manager* manager; //
    map< string , SDL_Texture* > textures; // vector lưu các texture
    map<string, Mix_Chunk*> sounds;
    map<string, Mix_Music*> music;
    map<string, TTF_Font*> fonts;

public:

    AssetManager(Manager* man);

    ~AssetManager();

    //Texture management
    void AddTexture(string id, const char* path);
    SDL_Texture* GetTexture(string id);

    //Audio management
    bool initAudio();
    void loadSound(string id, const char* path);
    void loadMusic(string id, const char* path);
    void playSound(string id, int loop);
    void playMusic(string id, int loop);
    void pauseMusic();
    void stopSound(string id);
    void quitAudio();

    //Text management
    bool initTTF();
    void loadFont(string id, const char* path, int fsize);
    TTF_Font* GetFont(string id);
    void quitTTF();
};

#endif