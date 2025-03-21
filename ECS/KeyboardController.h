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
                if (check[SDL_SCANCODE_DOWN]) transform->velocity.y = 1;
                if (check[SDL_SCANCODE_RIGHT]) transform->velocity.x = 1;
                if (check[SDL_SCANCODE_LEFT]) transform->velocity.x = -1;
            }

            else if ( Game::event.type == SDL_KEYUP ){
                transform->setVelocity(0,0);
            }
        }

};

#endif