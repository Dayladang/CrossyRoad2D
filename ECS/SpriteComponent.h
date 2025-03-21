#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Components.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\Game.h"

class SpriteComponent : public Component {

private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

public:
    SpriteComponent() = default;

    SpriteComponent(const char* link){
        setTex(link);
    }

    ~SpriteComponent(){
        SDL_DestroyTexture(texture);
    }

    void setTex(const char* link){
        texture = IMG_LoadTexture(Game::renderer, link);
    }

    void init () override {

        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0; 
        srcRect.w = transform->width;    
        srcRect.h = transform->height;    
    }

    void update() override {
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;

        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;

        if (destRect.x < 0 - 100) {
            transform->position.x = WIDTH + 100; // Đặt lại vị trí x để xuất hiện từ phía bên trái
        }
        else if (destRect.x > WIDTH + 100){
            transform->position.x = 0 - 100;
        }
    }

    void draw () override {
        SDL_RenderCopy(Game::renderer, texture, NULL, &destRect);
    }
};

#endif