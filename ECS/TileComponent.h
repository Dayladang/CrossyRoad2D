#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ECS.h"
#include <SDL2/SDL.h>

class TileComponent : public Component {

public :

    SDL_Texture* texture;
    SDL_Rect srcRect, destRect; // 
    Vector2D position; // vị trí của tile trên map
    
    TileComponent() = default;

    ~TileComponent(){
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, const char* path){
        texture = IMG_LoadTexture(Game::renderer, path);
        if (!texture){
            cerr << "loi khoi tao Texture" << SDL_GetError() << endl;
        }

        position.x = xpos; // 
        position.y = ypos;

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tsize; //kích thước của tile gốc 

        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = destRect.h = tsize * tscale; //kích thước của tile sau khi scale
        
    }

    void update() override {
        destRect.x = position.x - Game::screen.x; // di chuyển theo sreen
        destRect.y = position.y - Game::screen.y;
    }

    void draw() override {
        SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
    }



};

#endif