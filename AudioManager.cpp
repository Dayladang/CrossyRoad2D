#include "AudioManager.h"

AudioManager::AudioManager(){}

AudioManager::~AudioManager(){}

bool AudioManager::initAudio(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        cerr << "loi SDL_mixer: %s\n", Mix_GetError();
        return false;
    }
    return true;
}

void AudioManager::loadSound(string id, const char* path){
    Mix_Chunk* chunk = Mix_LoadWAV(path);
    sounds[id] = chunk;
}

void AudioManager::loadMusic(string id, const char* path){
    Mix_Music* musicsound = Mix_LoadMUS(path);
    music[id] = musicsound;
}

void AudioManager::playSound(string id, int loop){
    Mix_Chunk* chunk = sounds[id];
    Mix_PlayChannel(-1, chunk, loop);
}

void AudioManager::playMusic(string id, int loop){
    Mix_Music* musicsound = music[id];
    Mix_PlayMusic(musicsound, loop);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2); // giảm âm lượng nhạc nền
}

void AudioManager::pauseMusic(){
    if (Mix_PlayingMusic()){
        Mix_PauseMusic();
    }
}

void AudioManager::stopSound(string id){
    Mix_HaltChannel(-1);
}

void AudioManager::quitAudio(){
    Mix_HaltMusic();
    for (auto& s : sounds){
        Mix_FreeChunk(s.second);
    }
    for (auto& m : music){
        Mix_FreeMusic(m.second);
    }
    Mix_CloseAudio();
    Mix_Quit();
}