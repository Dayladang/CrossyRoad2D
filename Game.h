#ifndef GRAPHIC_H // ten cua thu vien //2
#define GRAPHIC_H

#include <iostream>
#include "C:\Users\ADMIN\OneDrive\Documents\coding stuff\DUT_CAN_TRI\src\include\SDL2\SDL.h"
#include "C:\Users\ADMIN\OneDrive\Documents\coding stuff\DUT_CAN_TRI\src\include\SDL2\SDL_image.h"

const int WIDTH = 800;
const int HEIGHT = 640;
const char* WINDOW_TITLE = " Game Time !";

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
private:
    int cnt = WIDTH;
    bool isRunning;
    SDL_Window* window;   
};
#endif// GRAPHIC_H