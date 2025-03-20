#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {

public :
    TransformComponent *transform;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {

            if ( Game::event.type == SDL_KEYDOWN ){
                const Uint8* check = SDL_GetKeyboardState(NULL);

                if (check[SDL_SCANCODE_UP]) transform->velocity.y = -1;
                else if (check[SDL_SCANCODE_DOWN]) transform->velocity.y = 1;
                else if (check[SDL_SCANCODE_RIGHT]) transform->velocity.x = 1;
                else if (check[SDL_SCANCODE_LEFT]) transform->velocity.x = -1;
            }

            else if ( Game::event.type == SDL_KEYUP ){
                const Uint8* check = SDL_GetKeyboardState(NULL); // reset về 0 để vật không bị trôi

                if (check[SDL_SCANCODE_UP]){
                    transform->velocity.y = 0;
                    transform->velocity.x = 0;
                } 
                else if (check[SDL_SCANCODE_DOWN]){
                    transform->velocity.y = 0;
                    transform->velocity.x = 0;
                } 
                else if (check[SDL_SCANCODE_RIGHT]){
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                } 
                else if (check[SDL_SCANCODE_LEFT]){
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                } 
                //transform->setVelocity(0,0);
            }
        }

};

#endif