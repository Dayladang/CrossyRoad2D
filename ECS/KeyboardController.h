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

                if (check[SDL_SCANCODE_UP] || check[SDL_SCANCODE_W]) transform->velocity.y = -1;
                if (check[SDL_SCANCODE_DOWN] || check[SDL_SCANCODE_S]) transform->velocity.y = 1;
                if (check[SDL_SCANCODE_RIGHT] || check[SDL_SCANCODE_D]) transform->velocity.x = 1;
                if (check[SDL_SCANCODE_LEFT] || check[SDL_SCANCODE_A]) transform->velocity.x = -1;
                if (check[SDL_SCANCODE_RSHIFT]) transform->speed = 3;
            }

            else if ( Game::event.type == SDL_KEYUP ){
                const Uint8* check = SDL_GetKeyboardState(NULL);

                if (check[SDL_SCANCODE_UP] || check[SDL_SCANCODE_W]) transform->setVelocity(0,0);
                if (check[SDL_SCANCODE_DOWN] || check[SDL_SCANCODE_S]) transform->setVelocity(0,0);
                if (check[SDL_SCANCODE_RIGHT] || check[SDL_SCANCODE_D]) transform->setVelocity(0,0);
                if (check[SDL_SCANCODE_LEFT] || check[SDL_SCANCODE_A]) transform->setVelocity(0,0);
                if (check[SDL_SCANCODE_RSHIFT]) transform->speed = 1;
            }
        }

};

#endif