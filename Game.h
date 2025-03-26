#ifndef GRAPHIC_H // ten cua thu vien //2
#define GRAPHIC_H

#include <iostream>
#include <vector>
#include "C:\Users\ADMIN\OneDrive\Documents\coding stuff\DUT_CAN_TRI\src\include\SDL2\SDL.h"
#include "C:\Users\ADMIN\OneDrive\Documents\coding stuff\DUT_CAN_TRI\src\include\SDL2\SDL_image.h"

using namespace std;

const int WIDTH = 1024;
const int HEIGHT = 1024;
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

    static void AddTile(int srcX, int srcY, int xpos, int ypos);
    static SDL_Renderer* renderer; // renderer tổng, sẽ tồn tại đến khi chương trình két thúc //5 luon
    static SDL_Event event;
    static vector<ColliderComponent*> colliders;

private:
    bool isRunning;
    SDL_Window* window;   
};
#endif// GRAPHIC_H