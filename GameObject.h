#ifndef GAMEOBJECT_H //4
#define GAMEOBJECT_H

#include "Game.h"

class GameObject {

public:
    GameObject(const char* link, int x, int y);
    ~GameObject();

    void Update();
    void Render();

private:

    int xpos;
    int ypos;
    
    SDL_Texture* objTeture;
    SDL_Rect srcRect, destRect;

};

#endif