#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include "../ScoreSystem.h"

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

                if (Game::UIwriteName) {

                    const Uint8* check = SDL_GetKeyboardState(NULL);

                    if (check[SDL_SCANCODE_BACKSPACE] && Game::UIwriteName && !Game::playerName.empty()) {
                        Game::playerName.pop_back(); // xóa kí tự cuối cùng trong tên nếu ấn phím backspace
                    }
                    if (check[SDL_SCANCODE_RETURN] && !Game::playerName.empty()) {
                        Game::leaderBoard->addPlayer(Game::playerName, Game::currentScore);
                        Game::leaderBoard->saveToFile(); // luu lai du lieu vao file leaderboard.txt
                        Game::resetGame(); // nhập tên xong thì reset
                    }
                    if (check[SDL_SCANCODE_ESCAPE]) Game::isRunning = false;
                }
                else {
                    const Uint8* check = SDL_GetKeyboardState(NULL);

                    if (check[SDL_SCANCODE_ESCAPE]) Game::isRunning = false; // bấm esc để thoát game

                    if (!Game::playButtonClickedUp) return ; // nếu chưa bấm nút chơi thì chưa được di chuyển return luôn vì vẫn phải bắt chuột để nhấn chơi nên làm thế này              

                    if (check[SDL_SCANCODE_W]) {
                        transform->velocity.y = -1;
                        sprite->Play("Backwalk");
                        Game::assets->playSound("chickensound", 0);
                    }
                    if (check[SDL_SCANCODE_S]) {
                        transform->velocity.y = 1;
                        sprite->Play("Frontwalk");
                        Game::assets->playSound("chickensound", 0);
                    }
                    if (check[SDL_SCANCODE_D]) {
                        transform->velocity.x = 1;
                        sprite->Play("Rightwalk");
                        Game::assets->playSound("chickensound", 0);
                    } 
                    if (check[SDL_SCANCODE_A]) {
                        transform->velocity.x = -1;
                        sprite->Play("Leftwalk");
                        Game::assets->playSound("chickensound", 0);
                    }
                    if (check[SDL_SCANCODE_RSHIFT]) {
                        transform->speed = 3;
                    }
                }
            }

            else if ( Game::event.type == SDL_KEYUP ){

                if (!Game::playButtonClickedUp) return ;
                
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

                    //cout << mouseX << " " << mouseY << endl;

                    if (!Game::LeaderBoardButtonUp) {
                        if (mouseX >= Game::playButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= Game::playButton->getComponent<TransformComponent>().position.x + Game::playButton->getComponent<TransformComponent>().width &&
                        mouseY >= Game::playButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= Game::playButton->getComponent<TransformComponent>().position.y + Game::playButton->getComponent<TransformComponent>().height) {

                            Game::playButtonClickedDown = true;

                        }
                    }

                    if (Game::UIwriteName) { // khi màn hình thua mở ra 
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát màn hình thua
                            Game::exitGameloseDown = true;
                        }
                    }

                    if (Game::LeaderBoardButtonUp) {
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát leaderboard
                            
                            Game::exitLeaderBoardDown = true;

                        }
                    }

                    if (!Game::LeaderBoardButtonUp) {
                        if (mouseX >= Game::LeaderBoardButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= Game::LeaderBoardButton->getComponent<TransformComponent>().position.x + Game::LeaderBoardButton->getComponent<TransformComponent>().width &&
                        mouseY >= Game::LeaderBoardButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= Game::LeaderBoardButton->getComponent<TransformComponent>().position.y + Game::LeaderBoardButton->getComponent<TransformComponent>().height) {

                            Game::LeaderBoardButtonDown = true;

                        }
                    }
                }        
                
            }

            if (Game::event.type == SDL_MOUSEBUTTONUP) {
                if (Game::event.button.button == SDL_BUTTON_LEFT) {
                    int ngumouseX, ngumouseY;
                    SDL_GetMouseState(&ngumouseX, &ngumouseY); // đây là lấy tọa độ chuột trong của sổ

                    int mouseX = ngumouseX + Game::screen.x;// lấy tọa độ chuột trong map
                    int mouseY = ngumouseY + Game::screen.y;

                    //cout << mouseX << " " << mouseY << endl;

                    if (!Game::LeaderBoardButtonUp) {
                        if (mouseX >= Game::playButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= Game::playButton->getComponent<TransformComponent>().position.x + Game::playButton->getComponent<TransformComponent>().width &&
                        mouseY >= Game::playButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= Game::playButton->getComponent<TransformComponent>().position.y + Game::playButton->getComponent<TransformComponent>().height) {

                            Game::playButtonClickedDown = false;
                            Game::playButtonClickedUp = true; // that nút ra thì bắt đầu game

                        }
                    }

                    if (Game::UIwriteName) { // khi màn hình thua mở ra 
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát màn hình thua
                            
                            Game::exitGameloseDown = false;
                            Game::exitGameloseUp = true;
                            Game::resetGame(); // reset game khi thả nút thoát màn hình thua

                        }
                    }

                    if (Game::LeaderBoardButtonUp) {
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát leaderboard
                            
                            Game::exitLeaderBoardDown = false;
                            Game::exitLeaderBoardUp = true;

                            Game::playButtonClickedUp = false; //reset lại trạng thái nút chơi chưa được bấm
                            Game::LeaderBoardButtonUp = false; 
                            Game::exitLeaderBoardUp = false; // reset lại trạng thái nút hiện leaderboard chưa được bấm

                        }
                    }

                    if (!Game::playButtonClickedUp) {
                        if (mouseX >= Game::LeaderBoardButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= Game::LeaderBoardButton->getComponent<TransformComponent>().position.x + Game::LeaderBoardButton->getComponent<TransformComponent>().width &&
                        mouseY >= Game::LeaderBoardButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= Game::LeaderBoardButton->getComponent<TransformComponent>().position.y + Game::LeaderBoardButton->getComponent<TransformComponent>().height) {

                            Game::LeaderBoardButtonDown = false;
                            Game::LeaderBoardButtonUp = true;

                        }
                    }
                }        
            }

            if (Game::event.type == SDL_TEXTINPUT) {
                if (Game::UIwriteName && !Game::exitGameloseUp) {

                    char c = Game::event.text.text[0]; //lấy kí tự vừa nhập vào

                    if (c == ' ' && Game::playerName.size() < 20) {
                        Game::playerName += '_'; // thay space bằng "_"
                    }
                    else if (c != '\0' && Game::playerName.size() < 20) {
                        Game::playerName += c; // ký tự khác giữ nguyên
                    }
                }
            }
        }

};

#endif