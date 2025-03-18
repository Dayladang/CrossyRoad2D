#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Components.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\Game.h"

class SpriteComponent : public Component {

private:
    PositionComponent* position;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

public:
    SpriteComponent() = default;
    SpriteComponent(const char* link){
        texture = IMG_LoadTexture(Game::renderer, link);
    }

    void init () override {

        position = &entity->getComponent<PositionComponent>();

        srcRect.x = srcRect.y = 0;
        destRect.w = srcRect.w = 75; 
        destRect.h = srcRect.h = 50;    
    }

    void update() override {
        destRect.x = position->x();
        destRect.y = position->y();

        if (destRect.x < 0 - 100) {
            position->x(WIDTH + 100); // Đặt lại vị trí x để xuất hiện từ phía bên trái
        }
    }

    void draw () override {
        SDL_RenderCopy(Game::renderer, texture, NULL, &destRect);
    }
};

#endif