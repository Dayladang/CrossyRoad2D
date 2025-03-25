#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {

public :
    TransformComponent *transform;
    SpriteComponent* sprite;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override {

            if ( Game::event.type == SDL_KEYDOWN ){
                const Uint8* check = SDL_GetKeyboardState(NULL);

                if (check[SDL_SCANCODE_W]) {
                    transform->velocity.y = -1;
                    sprite->Play("Backwalk");
                }
                if (check[SDL_SCANCODE_S]) {
                    transform->velocity.y = 1;
                    sprite->Play("Frontwalk");
                }
                if (check[SDL_SCANCODE_D]) {
                    transform->velocity.x = 1;
                    sprite->Play("Rightwalk");
                } 
                if (check[SDL_SCANCODE_A]) {
                    transform->velocity.x = -1;
                    sprite->Play("Leftwalk");
                }
                if (check[SDL_SCANCODE_RSHIFT]) transform->speed = 3;
            }

            else if ( Game::event.type == SDL_KEYUP ){
                transform->velocity.Zero();
                sprite->Play("Idle");

                const Uint8* check = SDL_GetKeyboardState(NULL);
 
                if (check[SDL_SCANCODE_RSHIFT]) transform->speed = 1;
            }
        }

};

#endif