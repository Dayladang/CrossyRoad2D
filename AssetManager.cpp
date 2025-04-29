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

bool AssetManager::initAudio(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        cerr << "loi SDL_mixer: %s\n", Mix_GetError();
        return false;
    }
    return true;
}

void AssetManager::loadSound(string id, const char* path){
    Mix_Chunk* chunk = Mix_LoadWAV(path);
    sounds[id] = chunk;
}

void AssetManager::loadMusic(string id, const char* path){
    Mix_Music* musicsound = Mix_LoadMUS(path);
    music[id] = musicsound;
}

void AssetManager::playSound(string id, int loop){
    Mix_Chunk* chunk = sounds[id];
    Mix_PlayChannel(-1, chunk, loop);
}

void AssetManager::playMusic(string id, int loop){
    Mix_Music* musicsound = music[id];
    Mix_PlayMusic(musicsound, loop);
    //x_VolumeMusic(MIX_MAX_VOLUME / 2); // giảm âm lượng nhạc nền
}

void AssetManager::pauseMusic(){
    if (Mix_PlayingMusic()){
        Mix_PauseMusic();
    }
}

void AssetManager::resumeMusic() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void AssetManager::stopSound(string id){
    Mix_HaltChannel(-1);
}

void AssetManager::MuteMusicAndSound() {
    Mix_VolumeMusic(0);
    Mix_Volume(-1, 0);
}

void AssetManager::unMuteMusicAndSound() {
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_Volume(-1, MIX_MAX_VOLUME / 2);
}

bool AssetManager::initTTF(){
    if (TTF_Init() == -1){
        cerr << "loi khoi tao ttf %s\n" << TTF_GetError();
        return false;
    }
    return true;
}

void AssetManager::loadFont(string id, const char* path, int fsize){
    TTF_Font* font = TTF_OpenFont(path, fsize);
    if (font == NULL){
        cerr << "loi mo font %s\n" << TTF_GetError();
    }
    fonts[id] = font;
}

TTF_Font* AssetManager::GetFont(string id){
    return fonts[id];
}

void AssetManager::quit(){
    //quit mixer
    Mix_HaltMusic();
    for (auto& s : sounds){
        Mix_FreeChunk(s.second);
    }
    for (auto& m : music){
        Mix_FreeMusic(m.second);
    }
    Mix_CloseAudio();
    Mix_Quit();

    //quit ttf
    for (auto& f : fonts){
        TTF_CloseFont(f.second);
    }
    TTF_Quit();

    //quit image
    for(auto& t : textures) {
        SDL_DestroyTexture(t.second);
    }
    IMG_Quit();
}