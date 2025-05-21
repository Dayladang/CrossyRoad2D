#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include "../ScoreSystem.h"
#include "../ButtonManager.h"

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

            else if ( Game::event.type == SDL_QUIT) {
                ButtonManager::quitGameUp = true;
                Game::assets->playSound("click_down", 0);
            }

            else if ( Game::event.type == SDL_KEYDOWN ){

                if (Game::UIwriteName || ButtonManager::isPausedUp) {

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

                    if (check[SDL_SCANCODE_ESCAPE]) {
                        if (ButtonManager::quitGameUp) ButtonManager::quitGameUp = false; //nếu đang trong màn hình quit thì bấm esc phát nữa để thoát màn hình quit
                        else ButtonManager::quitGameUp = true;
                    }

                    if (!ButtonManager::playButtonClickedUp) return ; // nếu chưa bấm nút chơi thì chưa được di chuyển return luôn vì vẫn phải bắt chuột để nhấn chơi nên làm thế này              

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

                if (!ButtonManager::playButtonClickedUp) return ;
                
                const Uint8* check = SDL_GetKeyboardState(NULL);
                
                transform->velocity.Zero();
                sprite->Play("Idle");
                Game::assets->stopSound("chickensound");                
 
                if (!check[SDL_SCANCODE_RSHIFT]) transform->speed = 1;
            }

            else if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
                if (Game::event.button.button == SDL_BUTTON_LEFT) {
                    int ngumouseX, ngumouseY;
                    SDL_GetMouseState(&ngumouseX, &ngumouseY); // đây là lấy tọa độ chuột trong của sổ

                    int mouseX = ngumouseX + Game::screen.x;// lấy tọa độ chuột trong map
                    int mouseY = ngumouseY + Game::screen.y;

                    // cout << mouseX << " " << mouseY << endl;

                    if (!ButtonManager::LeaderBoardButtonUp && !ButtonManager::playButtonClickedUp && !Game::UIwriteName) {
                        if (mouseX >= ButtonManager::playButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::playButton->getComponent<TransformComponent>().position.x + ButtonManager::playButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::playButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::playButton->getComponent<TransformComponent>().position.y + ButtonManager::playButton->getComponent<TransformComponent>().height) {

                            ButtonManager::playButtonClickedDown = true;
                            Game::assets->playSound("click_down", 0);

                        }
                    }

                    if (Game::UIwriteName) { // khi màn hình thua mở ra 

                        auto& tmp = Game::exitWriteName->getComponent<TransformComponent>();

                        if (mouseX >= tmp.position.x + 16 && mouseX <= tmp.position.x + 54 && 
                            mouseY >= tmp.position.y + 3 && mouseY <= tmp.position.y + 35) { // nằm trong khoảng của dấu thoát màn hình thua
                            ButtonManager::exitGameloseDown = true;
                            Game::assets->playSound("click_down", 0);
                        }
                    }

                    if (ButtonManager::LeaderBoardButtonUp) {
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát leaderboard
                            
                            ButtonManager::exitLeaderBoardDown = true;
                            Game::assets->playSound("click_down", 0);

                        }
                    }

                    if (!ButtonManager::LeaderBoardButtonUp && !ButtonManager::playButtonClickedUp) {
                        if (mouseX >= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.x + ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.y + ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().height) {

                            ButtonManager::LeaderBoardButtonDown = true;
                            Game::assets->playSound("click_down", 0);

                        }
                    }

                    if (ButtonManager::playButtonClickedUp && !ButtonManager::quitGameUp && !Game::UIwriteName && !ButtonManager::LeaderBoardButtonUp) {
                        if (mouseX >= ButtonManager::PauseButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::PauseButton->getComponent<TransformComponent>().position.x + ButtonManager::PauseButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::PauseButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::PauseButton->getComponent<TransformComponent>().position.y + ButtonManager::PauseButton->getComponent<TransformComponent>().height) {

                            ButtonManager::isPausedDown = true;
                            Game::assets->playSound("click_down", 0);

                        }
                    }

                    if (ButtonManager::quitGameUp) {

                        auto& quitPos = ButtonManager::quitGame->getComponent<TransformComponent>();

                        if (mouseX >= quitPos.position.x + 177 && mouseX <= quitPos.position.x + 239 &&
                            mouseY >= quitPos.position.y + 50 && mouseY <= quitPos.position.y + 62) { // nằm trong khoảng của dấu thoát leaderboard
                            
                            ButtonManager::quitGameNoDown = true;
                            Game::assets->playSound("click_down", 0);

                        }
                        else if (mouseX >= quitPos.position.x + 272 && mouseX <= quitPos.position.x + 334 &&
                                mouseY >= quitPos.position.y + 50 && mouseY <= quitPos.position.y + 62) {

                            ButtonManager::quitGameYesDown = true;
                            Game::assets->playSound("click_down", 0);

                        }
                    }

                    if (!ButtonManager::playButtonClickedUp) {
                        if (mouseX >= ButtonManager::mutedButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::mutedButton->getComponent<TransformComponent>().position.x + ButtonManager::mutedButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::mutedButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::mutedButton->getComponent<TransformComponent>().position.y + ButtonManager::mutedButton->getComponent<TransformComponent>().height) {

                            if (ButtonManager::MutedButtonUp) {
                                ButtonManager::MutedButtonDown = true;
                            }
                            else if (ButtonManager::unMutedButtonUp) {
                                ButtonManager::unMutedButtonDown = true;
                            }
                            Game::assets->playSound("click_down", 0);

                        }
                    }
                }        
                
            }

            else if (Game::event.type == SDL_MOUSEBUTTONUP) {
                if (Game::event.button.button == SDL_BUTTON_LEFT) {
                    int ngumouseX, ngumouseY;
                    SDL_GetMouseState(&ngumouseX, &ngumouseY); // đây là lấy tọa độ chuột trong của sổ

                    int mouseX = ngumouseX + Game::screen.x;// lấy tọa độ chuột trong map
                    int mouseY = ngumouseY + Game::screen.y;

                    // cout << mouseX << " " << mouseY << endl;

                    if (!ButtonManager::LeaderBoardButtonUp && !ButtonManager::playButtonClickedUp && !Game::UIwriteName) {
                        if (mouseX >= ButtonManager::playButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::playButton->getComponent<TransformComponent>().position.x + ButtonManager::playButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::playButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::playButton->getComponent<TransformComponent>().position.y + ButtonManager::playButton->getComponent<TransformComponent>().height) {

                            ButtonManager::playButtonClickedDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::playButtonClickedUp = true; // that nút ra thì bắt đầu game

                        }
                    }

                    if (Game::UIwriteName) { // khi màn hình thua mở ra 

                        auto& tmp = Game::exitWriteName->getComponent<TransformComponent>();

                        if (mouseX >= tmp.position.x + 16 && mouseX <= tmp.position.x + 54 && 
                            mouseY >= tmp.position.y + 3 && mouseY <= tmp.position.y + 35) { // nằm trong khoảng của dấu thoát màn hình thua
                            
                            ButtonManager::exitGameloseDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::exitGameloseUp = true;
                            Game::resetGame(); // reset game khi thả nút thoát màn hình thua

                        }
                    }

                    if (ButtonManager::LeaderBoardButtonUp) {
                        if (mouseX >= 378 && mouseX <= 418 && mouseY >= 607 && mouseY <= 642) { // nằm trong khoảng của dấu thoát leaderboard
                            
                            ButtonManager::exitLeaderBoardDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::exitLeaderBoardUp = true;

                            ButtonManager::playButtonClickedUp = false; //reset lại trạng thái nút chơi chưa được bấm
                            ButtonManager::LeaderBoardButtonUp = false; 
                            ButtonManager::exitLeaderBoardUp = false; // reset lại trạng thái nút hiện leaderboard chưa được bấm

                        }
                    }

                    if (!ButtonManager::playButtonClickedUp && !ButtonManager::playButtonClickedUp) {
                        if (mouseX >= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.x + ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().position.y + ButtonManager::LeaderBoardButton->getComponent<TransformComponent>().height) {

                            ButtonManager::LeaderBoardButtonDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::LeaderBoardButtonUp = true;

                        }
                    }

                    if (ButtonManager::playButtonClickedUp && !ButtonManager::quitGameUp && !Game::UIwriteName && !ButtonManager::LeaderBoardButtonUp) {
                        if (mouseX >= ButtonManager::PauseButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::PauseButton->getComponent<TransformComponent>().position.x + ButtonManager::PauseButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::PauseButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::PauseButton->getComponent<TransformComponent>().position.y + ButtonManager::PauseButton->getComponent<TransformComponent>().height) {

                            ButtonManager::isPausedDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::isPausedUp = true;

                        }
                    }

                    if (ButtonManager::quitGameUp) {

                        auto& quitPos = ButtonManager::quitGame->getComponent<TransformComponent>();

                        if (mouseX >= quitPos.position.x + 177 && mouseX <= quitPos.position.x + 239 &&
                            mouseY >= quitPos.position.y + 50 && mouseY <= quitPos.position.y + 62) { // nằm trong khoảng của dấu thoát leaderboard
                            
                            ButtonManager::quitGameNoDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::quitGameNoUp = true;

                        }
                        else if (mouseX >= quitPos.position.x + 272 && mouseX <= quitPos.position.x + 334 &&
                                mouseY >= quitPos.position.y + 50 && mouseY <= quitPos.position.y + 62) {

                            ButtonManager::quitGameYesDown = false;
                            Game::assets->playSound("click_up", 0);
                            ButtonManager::quitGameYesUp = true;

                        }
                    }

                    if (!ButtonManager::playButtonClickedUp) {
                        if (mouseX >= ButtonManager::mutedButton->getComponent<TransformComponent>().position.x &&
                        mouseX <= ButtonManager::mutedButton->getComponent<TransformComponent>().position.x + ButtonManager::mutedButton->getComponent<TransformComponent>().width &&
                        mouseY >= ButtonManager::mutedButton->getComponent<TransformComponent>().position.y &&
                        mouseY <= ButtonManager::mutedButton->getComponent<TransformComponent>().position.y + ButtonManager::mutedButton->getComponent<TransformComponent>().height) {
                            
                            if (ButtonManager::MutedButtonUp) {
                                ButtonManager::MutedButtonDown = false;
                                ButtonManager::unMutedButtonUp = true;
                                ButtonManager::MutedButtonUp = false;
                            }
                            else if (ButtonManager::unMutedButtonUp){
                                ButtonManager::unMutedButtonDown = false;
                                ButtonManager::MutedButtonUp = true;
                                ButtonManager::unMutedButtonUp = false;
                            }
                            Game::assets->playSound("click_up", 0);

                        }
                    }
                }        
            }

            else if (Game::event.type == SDL_TEXTINPUT) {
                if (Game::UIwriteName && !ButtonManager::exitGameloseUp) {

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