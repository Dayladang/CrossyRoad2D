#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Components.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\Game.h"
#include "Animation.h"
#include  <bits/stdc++.h>

using namespace std;

class SpriteComponent : public Component {

private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int speed = 100; // 100ms

public:

    int animIndex = 0; // chỉ số của animation

    map<const char*, Animation> animations; // Lưu trữ các animation

    SpriteComponent() = default;

    SpriteComponent(const char* link){
        setTex(link);
    }

    SpriteComponent(const char* link, bool isAnimated){

        animated = isAnimated;

        Animation idle = Animation(0, 2, 200);
        Animation leftwalk = Animation(1, 5, 100);
        Animation rightwalk = Animation(2, 5, 100);
        Animation frontwalk = Animation(3, 5, 100);
        Animation backwalk = Animation(4, 5, 100);

        animations.emplace("Idle", idle); // thêm animation vào map
        animations.emplace("Leftwalk", leftwalk);
        animations.emplace("Rightwalk", rightwalk);
        animations.emplace("Frontwalk", frontwalk);
        animations.emplace("Backwalk", backwalk);

        Play("Idle");

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

        if (animated){
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
        }

        srcRect.y = animIndex * transform->height; // đặt vị trí cuat y cho animation

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
        cout << "draw Sprite" << endl;
        SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
    }

    void Play(const char* animName){
        frames = (animations[animName]).frames;// animations[animName] la second
        animIndex = (animations[animName]).index;
        speed = (animations[animName]).speed; 
    }
};

#endif