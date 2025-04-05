#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

using namespace std;

class AudioManager {

public:
    AudioManager();
    ~AudioManager();

    bool initAudio();
    void loadSound(string id, const char* path);
    void loadMusic(string id, const char* path);
    void playSound(string id, int loop);
    void playMusic(string id, int loop);
    void pauseMusic();
    void stopSound(string id);
    void quitAudio();

private:
    map<string, Mix_Chunk*> sounds;
    map<string, Mix_Music*> music;
};

#endif