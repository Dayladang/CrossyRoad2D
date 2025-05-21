#include "ButtonManager.h"
// #include "Game.h"
#include "ECS/Components.h"

extern Manager manager; // sử dụng tiếp manager đã được khai báo ở Game.cpp
class Game;

ButtonManager::ButtonManager(){}
ButtonManager::~ButtonManager(){}

void ButtonManager::initButtons() {
    //thêm nút chơi
    Game::assets->AddTexture("play_button", "assets/images/Play_Button.png");
    Game::assets->AddTexture("play_button_clicked", "assets/images/PLay_Button_Clicked.png");
    playButton = &manager.addEntity(); //28
    playButton->addComponent<TransformComponent>((1024 - 100) / 2, 970, 100, 41, 1, 0, 0); 
    playButton->addComponent<SpriteComponent>("play_button");

    //thêm nút leaderboard
    Game::assets->AddTexture("leaderboard_button", "assets/images/leaderboard_button.png");
    Game::assets->AddTexture("leaderboard_button_clicked", "assets/images/leaderboard_button_clicked.png");
    LeaderBoardButton = &manager.addEntity();
    LeaderBoardButton->addComponent<TransformComponent>((1024 - 50) / 2 - 100, 970, 50, 41, 1, 0, 0);
    LeaderBoardButton->addComponent<SpriteComponent>("leaderboard_button"); 

    //thêm nút pause
    Game::assets->AddTexture("pause_button", "assets/images/pause_button.png");
    Game::assets->AddTexture("pause_button_clicked", "assets/images/pause_button_clicked.png");
    PauseButton = &manager.addEntity(); 
    PauseButton->addComponent<TransformComponent>(0, 0, 40, 40, 1, 0, 0);
    PauseButton->addComponent<SpriteComponent>("pause_button");

    Game::assets->AddTexture("pause_screen", "assets/images/pause_screen.png");
    Game::assets->AddTexture("pause_screen1", "assets/images/pause_screen1.png");
    Game::assets->AddTexture("pause_screen2", "assets/images/pause_screen2.png");
    Game::assets->AddTexture("pause_screen3", "assets/images/pause_screen3.png");
    PauseScreen = &manager.addEntity();
    PauseScreen->addComponent<TransformComponent>(0, 0, 50, 50, 1, 0, 0);
    PauseScreen->addComponent<SpriteComponent>("pause_screen");

    //thêm nút mute
    mutedButton = &manager.addEntity();
    Game::assets->AddTexture("unmuted_up", "assets/images/unmuted_button.png");
    Game::assets->AddTexture("unmuted_down", "assets/images/unmuted_button_clicked.png");
    Game::assets->AddTexture("muted_up", "assets/images/muted_button.png");
    Game::assets->AddTexture("muted_down", "assets/images/muted_button_clicked.png");
    mutedButton->addComponent<TransformComponent>((1024 - 50) / 2 + 100, 970, 50, 41, 1, 0, 0);
    mutedButton->addComponent<SpriteComponent>("unmuted_up");

    //thêm màn hình quit
    Game::assets->AddTexture("quit", "assets/images/quit.png");
    Game::assets->AddTexture("quit_yes", "assets/images/quit_yes.png");
    Game::assets->AddTexture("quit_no", "assets/images/quit_no.png");
    quitGame = &manager.addEntity();
    quitGame->addComponent<TransformComponent>(0, 0, 256, 80, 1, 0, 0);
    quitGame->addComponent<SpriteComponent>("quit");
}

void ButtonManager::drawButtons() {
    if (playButtonClickedUp) { 
        // PauseButton cũng được vẽ nếu bấm nút chơi
        if (!isPausedUp && !quitGameUp && !Game::UIwriteName) {
            // căn góc phải của màn hình
            auto& pb = PauseButton->getComponent<TransformComponent>();
            const int digit = 10;
            pb.position.x = Game::screen.x + WIDTH - pb.width - digit;
            pb.position.y = Game::screen.y + digit;

            if (isPausedDown) PauseButton->getComponent<SpriteComponent>().setTex("pause_button_clicked");
            else PauseButton->getComponent<SpriteComponent>().setTex("pause_button");
            PauseButton->draw();
        }

    }

    // vẽ button khi chưa thả nút ra  
    if (!playButtonClickedUp && !LeaderBoardButtonUp) {

        // Cập nhật vị trí các nút theo camera (giống cách làm của PauseButton)
        // Đặt các nút ở dưới màn hình
        const int buttonMargin = 20; // Khoảng cách từ đáy màn hình
        
        // Nút Play ở giữa dưới
        auto& playPos = playButton->getComponent<TransformComponent>();
        playPos.position.x = Game::screen.x + (WIDTH - playPos.width) / 2;
        playPos.position.y = Game::screen.y + HEIGHT - playPos.height - buttonMargin;
        
        // Nút Leaderboard ở bên trái của Play
        auto& lbPos = LeaderBoardButton->getComponent<TransformComponent>();
        lbPos.position.x = Game::screen.x + (WIDTH - lbPos.width) / 2 - 100;
        lbPos.position.y = Game::screen.y + HEIGHT - lbPos.height - buttonMargin;
        
        // Nút Mute ở bên phải của Play
        auto& mutePos = mutedButton->getComponent<TransformComponent>();
        mutePos.position.x = Game::screen.x + (WIDTH - mutePos.width) / 2 + 100;
        mutePos.position.y = Game::screen.y + HEIGHT - mutePos.height - buttonMargin;

        //vẽ nút play
        if (playButtonClickedDown) { // nếu nút được bấm
            playButton->getComponent<SpriteComponent>().setTex("play_button_clicked");
        }
        else { // nút khi chưa được bấm
            playButton->getComponent<SpriteComponent>().setTex("play_button");         
        }
        playButton->draw();

        //vẽ nút leaderboard
        if (LeaderBoardButtonDown) {
            LeaderBoardButton->getComponent<SpriteComponent>().setTex("leaderboard_button_clicked");
        }
        else {
            LeaderBoardButton->getComponent<SpriteComponent>().setTex("leaderboard_button");           
        }
        LeaderBoardButton->draw();

        //vẽ nút mute
        string tex;
        if (MutedButtonUp && !unMutedButtonUp) {
            tex = MutedButtonDown ? "muted_down" : "muted_up";
        }
        else if (unMutedButtonUp && !MutedButtonUp) {
            tex = unMutedButtonDown ? "unmuted_down" : "unmuted_up";
        }
        mutedButton->getComponent<SpriteComponent>().setTex(tex);
        mutedButton->draw();
    }

    if (isPausedUp) {
        //vẽ một khung bán trong suốt
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 180);
        SDL_Rect  blur = {0,0, WIDTH, HEIGHT};
        SDL_RenderFillRect(Game::renderer, &blur);

        //căn giữa 
        SDL_Texture* pauseTex = Game::assets->GetTexture("pause_screen");
        int texW, texH;
        //lấy kích thước của texture
        SDL_QueryTexture(pauseTex, NULL, NULL, &texW, &texH);
        SDL_Rect dst{(WIDTH  - texW) / 2, (HEIGHT - texH) / 2, texW, texH};

        auto& ps = PauseScreen->getComponent<TransformComponent>();
        ps.position.x = Game::screen.x + dst.x;
        ps.position.y = Game::screen.y + dst.y;
        ps.width = dst.w;
        ps.height = dst.h;

        // cout << ps.position.x << " " << ps.position.y << endl;

        //Vẽ thẳng lên renderer
        SDL_RenderCopy(Game::renderer, pauseTex, NULL, &dst);

    }

    if (quitGameUp) {
        SDL_Texture* tmp = Game::assets->GetTexture("quit");
        if (quitGameYesDown) {
            tmp = Game::assets->GetTexture("quit_yes");
        }
        else if (quitGameNoDown) {
            tmp = Game::assets->GetTexture("quit_no");
        }

        int texW, texH;
        SDL_QueryTexture(tmp, NULL, NULL, &texW, &texH);
        SDL_Rect dstRect = {(WIDTH - texW) / 2, (HEIGHT - texH) / 2, texW, texH};

        // Cập nhật vị trí của quitGame entity
        auto& qs = quitGame->getComponent<TransformComponent>();
        qs.position.x = Game::screen.x + dstRect.x;
        qs.position.y = Game::screen.y + dstRect.y;
        qs.width = dstRect.w;
        qs.height = dstRect.h;

        SDL_RenderCopy(Game::renderer, tmp, NULL, &dstRect); 
    }
}

void ButtonManager::resetButtons() {
    playButtonClickedUp = false;
    exitGameloseUp = false;
    LeaderBoardButtonUp = false;
    exitLeaderBoardUp = false;
    quitGameUp = false;
    quitGameYesUp = false;
    quitGameNoUp = false;
}