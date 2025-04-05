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

            if ( Game::event.type == SDL_QUIT) Game::isRunning = false;

            else if ( Game::event.type == SDL_KEYDOWN ){
                const Uint8* check = SDL_GetKeyboardState(NULL);

                if (check[SDL_SCANCODE_W]) {
                    transform->velocity.y = -1;
                    sprite->Play("Backwalk");
                    Game::audio->playSound("chickensound", 0);
                }
                if (check[SDL_SCANCODE_S]) {
                    transform->velocity.y = 1;
                    sprite->Play("Frontwalk");
                    Game::audio->playSound("chickensound", 0);
                }
                if (check[SDL_SCANCODE_D]) {
                    transform->velocity.x = 1;
                    sprite->Play("Rightwalk");
                    Game::audio->playSound("chickensound", 0);
                } 
                if (check[SDL_SCANCODE_A]) {
                    transform->velocity.x = -1;
                    sprite->Play("Leftwalk");
                    Game::audio->playSound("chickensound", 0);
                }
                if (check[SDL_SCANCODE_RSHIFT]) transform->speed = 3;
                if (check[SDL_SCANCODE_ESCAPE]) Game::isRunning = false;
            }

            else if ( Game::event.type == SDL_KEYUP ){
                
                const Uint8* check = SDL_GetKeyboardState(NULL);
                
                transform->velocity.Zero();
                sprite->Play("Idle");
                Game::audio->stopSound("chickensound");                
 
                if (!check[SDL_SCANCODE_RSHIFT]) transform->speed = 1;
            }
        }

};

#endif