#ifndef GRAPHIC_H // ten cua thu vien //2
#define GRAPHIC_H

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "AssetManager.h" 

using namespace std;

const int WIDTH = 512;
const int HEIGHT = 512;
const char* WINDOW_TITLE = " Game Time !";

class ColliderComponent;

class Game{

public:
    Game();
    ~Game();

    void initSDL(const int WIDTH, const int HEIGHT, const char* WINDOW_TITLE);
    void handleEvents();
    void update();
    bool running(){ return isRunning;};
    void render();
    void quit();

    static SDL_Renderer* renderer; // renderer tổng, sẽ tồn tại đến khi chương trình két thúc //5 luon
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect screen;
    static AssetManager* assets;

    enum groupLabels : size_t { // size_t được định nghĩa trong ECS.h là Group
        groupMap,
        groupPlayer,
        groupColliders,
        groupVehicles
    };

private:   
    SDL_Window* window;   
};
#endif// GRAPHIC_H