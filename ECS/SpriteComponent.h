#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "Components.h"
#include "Animation.h"
#include <string>
#include <map>
#include "../AssetManager.h"

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

    SpriteComponent(string id){
        setTex(id);
    }

    SpriteComponent(string id, bool isAnimated){

        animated = isAnimated;

        Animation idle = Animation(0, 2, 200);
        Animation leftwalk = Animation(1, 5, 100);
        Animation rightwalk = Animation(2, 5, 100);
        Animation frontwalk = Animation(3, 5, 100);
        Animation backwalk = Animation(4, 5, 100);
        Animation squash = Animation(5, 1, 100);

        animations.emplace("Idle", idle); // thêm animation vào map
        animations.emplace("Leftwalk", leftwalk);
        animations.emplace("Rightwalk", rightwalk);
        animations.emplace("Frontwalk", frontwalk);
        animations.emplace("Backwalk", backwalk);
        animations.emplace("Squash", squash);

        Play("Idle");

        setTex(id);
    }

    ~SpriteComponent(){
    }

    void setTex(string id){
        texture = Game::assets->GetTexture(id);
    }

    void init () override {

        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0; 
        srcRect.w = transform->width;    
        srcRect.h = transform->height;    
    }

    void update() override {

        if (animated){
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames); // chuyển đổi giữa các frame
        }

        srcRect.y = animIndex * transform->height; // đặt vị trí cuat y cho animation

        destRect.x = transform->position.x - Game::screen.x; // di chuyển tho screen với vân tốc của player 
        destRect.y = transform->position.y - Game::screen.y;

        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;

        if (transform->position.x < 0 - 100) {
            transform->position.x = 1024 + 100; // Đặt lại vị trí x để xuất hiện lại từ phía bên phải
        }
        else if (transform->position.x > 1024 + 100){
            transform->position.x = 0 - 100;
        }
    }

    void draw () override {
        SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);
    }

    void Play(const char* animName){
        frames = (animations[animName]).frames;// animations[animName] la second
        animIndex = (animations[animName]).index;
        speed = (animations[animName]).speed; 
    }
};

#endif