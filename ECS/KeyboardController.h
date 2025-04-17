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

            if ( Game::isSquashed){
                transform->velocity.Zero();
                return ;
            } 

            if ( Game::event.type == SDL_QUIT) Game::isRunning = false;

            else if ( Game::event.type == SDL_KEYDOWN ){

                if (!Game::playButtonClicked) return ; // nếu chưa bấm nút chơi thì chưa được di chuyển return luôn vì vẫn phải bắt chuột để nhấn chơi nên làm thế này

                const Uint8* check = SDL_GetKeyboardState(NULL);

                if (check[SDL_SCANCODE_W]) {
                    if (Game::isGamelose) return; // nếu đang ở màn hình thua thì không được di chuyển, vì vẫn phải bắt nút esc nên phải làm thế này
                    transform->velocity.y = -1;
                    sprite->Play("Backwalk");
                    Game::assets->playSound("chickensound", 0);
                }
                if (check[SDL_SCANCODE_S]) {
                    if (Game::isGamelose) return;
                    transform->velocity.y = 1;
                    sprite->Play("Frontwalk");
                    Game::assets->playSound("chickensound", 0);
                }
                if (check[SDL_SCANCODE_D]) {
                    if (Game::isGamelose) return;
                    transform->velocity.x = 1;
                    sprite->Play("Rightwalk");
                    Game::assets->playSound("chickensound", 0);
                } 
                if (check[SDL_SCANCODE_A]) {
                    if (Game::isGamelose) return;
                    transform->velocity.x = -1;
                    sprite->Play("Leftwalk");
                    Game::assets->playSound("chickensound", 0);
                }
                if (check[SDL_SCANCODE_RSHIFT]) transform->speed = 3;
                if (check[SDL_SCANCODE_ESCAPE]) Game::isRunning = false;
            }

            else if ( Game::event.type == SDL_KEYUP ){

                if (!Game::playButtonClicked) return ;
                
                const Uint8* check = SDL_GetKeyboardState(NULL);
                
                transform->velocity.Zero();
                sprite->Play("Idle");
                Game::assets->stopSound("chickensound");                
 
                if (!check[SDL_SCANCODE_RSHIFT]) transform->speed = 1;
            }

            if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
                if (Game::event.button.button == SDL_BUTTON_LEFT) {
                    int ngumouseX, ngumouseY;
                    SDL_GetMouseState(&ngumouseX, &ngumouseY); // đây là lấy tọa độ chuột trong của sổ

                    int mouseX = ngumouseX + Game::screen.x;// lấy tọa độ chuột trong map
                    int mouseY = ngumouseY + Game::screen.y;

                    cout << mouseX << " " << mouseY << endl;

                    if (mouseX >= Game::playButton->getComponent<TransformComponent>().position.x &&
                    mouseX <= Game::playButton->getComponent<TransformComponent>().position.x + Game::playButton->getComponent<TransformComponent>().width &&
                    mouseY >= Game::playButton->getComponent<TransformComponent>().position.y &&
                    mouseY <= Game::playButton->getComponent<TransformComponent>().position.y + Game::playButton->getComponent<TransformComponent>().height) {
                        Game::playButtonClicked = true; // đánh dấu đã nhấn nút chơi
                    }

                    if (Game::isGamelose) { // khi màn hình thua mở ra 
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát màn hình thua
                            Game* gameTmp;
                            gameTmp->resetGame();
                        }
                    }
                }        
                
            }
        }

};

#endif