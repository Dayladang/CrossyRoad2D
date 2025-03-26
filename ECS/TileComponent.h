#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ECS.h"
#include <SDL2/SDL.h>

class TileComponent : public Component {

public :

    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    
    TileComponent() = default;

    ~TileComponent(){
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, const char* path){
        texture = IMG_LoadTexture(Game::renderer, path);
        if (!texture){
            cerr << "loi khoi tao Texture" << SDL_GetError() << endl;
        }

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = 32; //kích thước của tile gốc 

        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = 32; //kích thước của tile sau khi scale
        destRect.h = 32;
    }

    void draw() override {
        //cout << "draw Tile" << endl;
        SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
    }



};

#endif