#include "Game.h"
#include "Map.cpp"//5
#include "ECS/Components.h"
#include "Vector2D.cpp"//8
#include "Collision.cpp"//10
#include "ECS/ECS.cpp" // 16
#include "AssetManager.cpp" //19 + 23
#include "ScoreSystem.cpp" // 27

using namespace std;

Map* gameMap;
Manager manager;//6

SDL_Renderer* Game::renderer = NULL; //5
SDL_Event Game::event;//9

//camera 
SDL_Rect Game::screen = {0, 0, WIDTH, HEIGHT}; //17

//màu chữ
SDL_Color black = {0, 0, 0, 255};
SDL_Color white = {255, 255, 255, 255};
LeaderBoard* Game::leaderBoard = new LeaderBoard(); // 34
ScoreSystem* Game::scoreSystem = new ScoreSystem(); // 27
int Game::currentScore = 0;// 33

bool Game::isSquashed = false; //26
Uint32 squashStartTime = 0; // 25

bool Game::playButtonClickedUp = false; // 28
bool Game::playButtonClickedDown = false; //32
Entity* Game::playButton = NULL;// 28
int logoPositionX = (1024 - 200) / 2 ; //29

bool Game::isLogoActive = true; // 31

bool Game::exitGameloseUp = false; // 32
bool Game::exitGameloseDown = false; // 32

bool Game::UIwriteName = false;// 30
string Game::playerName = ""; // 34
bool Game::isTypingName = false; // 34

bool Game::LeaderBoardButtonUp = false;// 37
bool Game::LeaderBoardButtonDown = false; // 37
bool Game::exitLeaderBoardUp = false;// 34
bool Game::exitLeaderBoardDown = false; // 37
Entity* Game::LeaderBoardButton = NULL; // 37

bool Game::isPausedUp = false; // 38
bool Game::isPausedDown = false; //38
Entity* Game::PauseButton = NULL; //38
Entity* Game::PauseScreen = NULL; //38

AssetManager* Game::assets = new AssetManager(&manager);//19 + 23

bool Game::isRunning = false;// 17
 
Entity& player = manager.addEntity();//6
Entity& scoreBoard = manager.addEntity(); //25
Entity& logo = manager.addEntity(); //28
Entity& writeScore = manager.addEntity(); //30
Entity& writeName = manager.addEntity(); // 34
Entity& writeName2 = manager.addEntity(); // 36
Entity& writeLeaderBoard = manager.addEntity(); // 37

vector<Entity*>& tiles = manager.getGroup(Game::groupMap); // tiles là một vector các entity trong nhóm groupMap
vector<Entity*>& players = manager.getGroup(Game::groupPlayer);
vector<Entity*>& colliders = manager.getGroup(Game::groupColliders);//18
vector<Entity*>& dangers = manager.getGroup(Game::groupDangers);//21
vector<Entity*>& vehicles = manager.getGroup(Game::groupVehicles); // 26

Game::Game(){}

Game::~Game(){}

void Game::initSDL(const int WIDTH, const int HEIGHT, const char* WINDOW_TITLE){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("loi INIT %s\n", SDL_GetError());
        isRunning = false;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("loi window %s\n", SDL_GetError());
        isRunning = false;
    }

    renderer = SDL_CreateRenderer(window, - 1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC );
    if (renderer == NULL){
        printf("loi render %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        isRunning = false;
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    } 

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)){
        printf("loi INIT IMAGE %s\n", IMG_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        isRunning = false;
    } 

    if (assets->initAudio() == false){
        isRunning = false;
    }

    if (!assets->initTTF()){
        isRunning = false;
    }

    isRunning = true;
      
    //thêm texture cho map
    assets->AddTexture("terrain", "assets/maptile/tilesheet.png");

    //thêm nhân vật
    assets->AddTexture("player", "assets/images/chick_total.png");
    assets->AddTexture("taxi", "assets/images/taxi.png");
    assets->AddTexture("truck", "assets/images/truck.png");
    assets->AddTexture("redtruck", "assets/images/redtruck.png");
    assets->AddTexture("ambulance", "assets/images/ambulance.png");
    assets->AddTexture("trainleft", "assets/images/trainleft.png");
    assets->AddTexture("train", "assets/images/train.png");
    assets->AddTexture("musclecar", "assets/images/musclecar.png");
    assets->AddTexture("hatchback", "assets/images/hatchback.png");
    assets->AddTexture("luxurycar", "assets/images/luxurycar.png");
    assets->AddTexture("jeep", "assets/images/jeep.png");
    assets->AddTexture("microcar", "assets/images/microcar.png");
    assets->AddTexture("minivan", "assets/images/minivan.png");
    assets->AddTexture("SUV", "assets/images/SUV.png");
    assets->AddTexture("wagon", "assets/images/wagon.png");

    //thêm màn hình điểm khi thua
    assets->AddTexture("losing_screen", "assets/images/losing_screen.png");
    assets->AddTexture("losing_screen_clicked", "assets/images/losing_screen_clicked.png");

    //thêm leaderboard
    assets->AddTexture("leaderboard", "assets/images/leaderboard.png");
    assets->AddTexture("leaderboard_clicked", "assets/images/leaderboard_clicked.png");

    //thêm âm thanh
    assets->loadMusic("thememusic", "assets/sound/thememusic.mp3");
    assets->loadSound("chickensound","assets/sound/chicken_sound.mp3");
    assets->loadSound("crashsound", "assets/sound/chicken_crashsound.mp3");

    //thêm font chữ
    assets->loadFont("font", "assets/fonts/pixelfont.ttf", 15);
    assets->loadFont("gameover_font", "assets/fonts/pixelfont.ttf", 40);
    assets->loadFont("Type_name", "assets/fonts/pixelfont.ttf", 12);
    scoreBoard.addComponent<MiniText>(10, 10, "", "font", black);

    //thêm logo
    assets->AddTexture("logo", "assets/images/logo.png");
    logo.addComponent<TransformComponent>(logoPositionX, 700, 200, 92, 1, 0, 0);
    logo.addComponent<SpriteComponent>("logo");

    //thêm nút chơi
    assets->AddTexture("play_button", "assets/images/Play_Button.png");
    assets->AddTexture("play_button_clicked", "assets/images/PLay_Button_Clicked.png");
    playButton = &manager.addEntity(); //28
    playButton->addComponent<TransformComponent>((1024 - 100) / 2, 970, 100, 41, 1, 0, 0); 
    playButton->addComponent<SpriteComponent>("play_button");

    //thêm nút leaderboard
    assets->AddTexture("leaderboard_button", "assets/images/leaderboard_button.png");
    assets->AddTexture("leaderboard_button_clicked", "assets/images/leaderboard_button_clicked.png");
    LeaderBoardButton = &manager.addEntity();
    LeaderBoardButton->addComponent<TransformComponent>((1024 - 50) / 2 - 100, 970, 50, 41, 1, 0, 0);
    LeaderBoardButton->addComponent<SpriteComponent>("leaderboard_button"); 

    //thêm nút pause
    assets->AddTexture("pause_button", "assets/images/pause_button.png");
    assets->AddTexture("pause_button_clicked", "assets/images/pause_button_clicked.png");
    PauseButton = &manager.addEntity(); 
    PauseButton->addComponent<TransformComponent>(0, 0, 40, 40, 1, 0, 0);
    PauseButton->addComponent<SpriteComponent>("pause_button");

    assets->AddTexture("pause_screen", "assets/images/pause_screen.png");
    assets->AddTexture("pause_screen1", "assets/images/pause_screen1.png");
    assets->AddTexture("pause_screen2", "assets/images/pause_screen2.png");
    assets->AddTexture("pause_screen3", "assets/images/pause_screen3.png");
    PauseScreen = &manager.addEntity();
    PauseScreen->addComponent<TransformComponent>(0, 0, 50, 50, 1, 0, 0);
    PauseScreen->addComponent<SpriteComponent>("pause_screen");

    //các khung viết chữ
    writeScore.addComponent<MiniText>(256, 256, "", "gameover_font", white);
    writeName.addComponent<MiniText>(0, 0, "", "Type_name", white);
    writeName2.addComponent<MiniText>(0, 0, "", "Type_name", white);
    writeLeaderBoard.addComponent<MiniText>(0, 0, "", "Type_name", white);

    //vẽ map
    gameMap = new Map("terrain", 1, 32);
    gameMap->LoadMap("assets/maptile/map1.map", 64, 32, 8); //14 x = 64 vì trong map1.map có 64 dòng 

    
    //vẽ nhân vật
    player.addComponent<TransformComponent>(512, 970, 24, 20, 1, 0, 0);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);

    //lấy màn hình bắt đầu bằng vị trí của nhân vật
    screen.x = player.getComponent<TransformComponent>().position.x - WIDTH / 2;
    screen.y = screen.h;

    //thêm hệ thống điểm
    int initialRow = player.getComponent<TransformComponent>().position.y / 32; // Giả sử 32 là kích thước ô
    scoreSystem->setLastPlayerRow(initialRow); 

    //lấy phương tiện từ file và thêm các tính năng
    fstream file("assets/maptile/vehicles.txt", ios::in);
    if (file.is_open()){
        string line;
        while (getline(file, line)){
            istringstream iss(line); // lấy string từ dòng 
            int x, y, w, h, scale, veloX, veloY; 
            string texID;

            iss >> x >> y >> w >> h >> scale >> veloX >> veloY;
            iss >> texID;

            Entity& vehicle = manager.addEntity();
            vehicle.addComponent<TransformComponent>(x, y, w, h, scale, veloX, veloY);
            vehicle.addComponent<SpriteComponent>(texID);
            vehicle.addComponent<ColliderComponent>(texID);
            vehicle.addGroup(groupVehicles);
        }

        file.close();
    }

    //phát âm thanh
    assets->playMusic("thememusic", -1);

}

void Game::handleEvents(){

    if (isSquashed) return;
    
    if (!isPausedUp) SDL_PollEvent(&event); 
    
    else {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {

                int ngumouseX, ngumouseY;
                SDL_GetMouseState(&ngumouseX, &ngumouseY); // đây là lấy tọa độ chuột trong của sổ

                int mouseX = ngumouseX + Game::screen.x;// lấy tọa độ chuột trong map
                int mouseY = ngumouseY + Game::screen.y;

                if (mouseX >= Game::PauseScreen->getComponent<TransformComponent>().position.x &&
                    mouseX <= Game::PauseScreen->getComponent<TransformComponent>().position.x + Game::PauseScreen->getComponent<TransformComponent>().width &&
                    mouseY >= Game::PauseScreen->getComponent<TransformComponent>().position.y &&
                    mouseY <= Game::PauseScreen->getComponent<TransformComponent>().position.y + Game::PauseScreen->getComponent<TransformComponent>().height) {

                        Game::isPausedUp = false;
                        //Game::exitPauseScreen = true;

                }
            }
        }
    }
   
}

void Game::update(){
    
    

    // chỉ update cho xe chạy nếu chưa nhấn nút chơi
    if (!playButtonClickedUp) {
        manager.refresh();//8
        manager.update();//6 
        return;
    } 

    // nếu ấn PauseButton thì tạm thời không cập nhật
    if (isPausedUp){
        assets->pauseMusic();
        return;
    }
    assets->resumeMusic();

    //mapcollider
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider; // lấy collider của player
    Vector2D playerPos = player.getComponent<TransformComponent>().position; // lấy vị trí của player để khi va chạm với vật cản thì trở về trạng thái chưa va chạm 

    manager.refresh();//8
    manager.update();//6

    //logo moving
    if (isLogoActive) {
        if (playButtonClickedUp) {
            logoPositionX += 8; // di chuyển logo sang phải

            if (logo.hasComponent<TransformComponent>()) {
                logo.getComponent<TransformComponent>().position.x = logoPositionX; 
            }
            
            if (logoPositionX > 1024) {
                isLogoActive = false; 
            }
        }
    }

    //mapcollider
    for (auto& c : colliders){
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider; // lấy collider của vật cản 

        if (Collision::AABB(cCol, playerCol)){
            player.getComponent<TransformComponent>().position = playerPos; // trở lại vị trí trc khi va chạm
        }
    }

    //camera
    screen.x = player.getComponent<TransformComponent>().position.x - WIDTH / 2; // làm cho screen di chuyển theo player
    screen.y = player.getComponent<TransformComponent>().position.y - HEIGHT / 2; 

    if (screen.x < 0) screen.x = 0; // giữ cho screen không di chuyển ra ngoài map
    if (screen.y < 0) screen.y = 0;
    if (screen.x > screen.w) screen.x = screen.w;
    if (screen.y > screen.h) screen.y = screen.h;

    //ghi điểm số 
    scoreSystem->updateScore(player.getComponent<TransformComponent>().position.y);
    scoreBoard.getComponent<MiniText>().SetLabelText(scoreSystem->getScoreText(), "font");

    // bắt sự kiện va chạm
    for(auto& v : vehicles){
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, v->getComponent<ColliderComponent>().collider)){
            player.getComponent<TransformComponent>().velocity.Zero();
            assets->playSound("crashsound", 0);
            player.getComponent<SpriteComponent>().Play("Squash");         
            squashStartTime = SDL_GetTicks64();
            isSquashed = true;           
            return ; 
        }
    }

    //nếu va chạm thì sẽ hiên trạng thái bị bẹp
    if (isSquashed) {
        // Kiểm tra nếu đã qua 1 giây kể từ khi bị đè
        if (SDL_GetTicks64() - squashStartTime > 1000) {
            player.getComponent<SpriteComponent>().Play("Idle");
            player.getComponent<TransformComponent>().position = {512, 970};
            UIwriteName = true;
            isSquashed = false;
            currentScore = scoreSystem->getScore();
            isTypingName = true;
            SDL_StartTextInput(); 
        }
        return; // Không xử lý logic khác khi đang ở trạng thái "bị bẹp"
    }

    for(auto& d : dangers){
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, d->getComponent<ColliderComponent>().collider)){
            player.getComponent<TransformComponent>().velocity.Zero();
            assets->playSound("crashsound", 0);
            player.getComponent<TransformComponent>().position = {512, 970};
            UIwriteName = true;
            currentScore = scoreSystem->getScore();
            isTypingName = true;
            SDL_StartTextInput();
        }
    }

    cout << "chicken position: (" 
    << player.getComponent<TransformComponent>().position.x << ", " 
    << player.getComponent<TransformComponent>().position.y << ")" << endl;

}

void Game::render(){   
    SDL_RenderClear(renderer); // ve map trc roi moi den nhan vat//5

    for (auto& t : tiles) t->draw();
    //for (auto& c : colliders) c->draw();

    for (auto& v : vehicles) v->draw();
    for (auto& d : dangers) d->draw();
    for (auto& p : players) p->draw(); 
    
    if (playButtonClickedUp) { 
        scoreBoard.draw(); // Score text

        // PauseButton cũng được vẽ nếu bấm nút chơi
        if (!isPausedUp) {
            // căn góc phải của màn hình
            auto& pb = PauseButton->getComponent<TransformComponent>();
            const int digit = 10;
            pb.position.x = screen.x + WIDTH - pb.width - digit;
            pb.position.y = screen.y + digit;

            if (isPausedDown) PauseButton->getComponent<SpriteComponent>().setTex("pause_button_clicked");
            else PauseButton->getComponent<SpriteComponent>().setTex("pause_button");
            PauseButton->draw();
        }

    }
    
    if (isLogoActive) {
        logo.draw();
    }
    
    // vẽ button khi chưa thả nút ra  
    if (!playButtonClickedUp && !LeaderBoardButtonUp) {
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
    }

    if (UIwriteName && !exitGameloseUp) {

        SDL_Texture* tmp = assets->GetTexture("losing_screen");
        if (exitGameloseDown) {
            tmp = assets->GetTexture("losing_screen_clicked");
        }
        
        int imgW = 300, imgH = 325;
        SDL_Rect dstRect = {(WIDTH - imgW) / 2, (HEIGHT - imgH) / 2, imgW, imgH};

        writeScore.getComponent<MiniText>().SetLabelText(to_string(currentScore), "gameover_font");
        writeScore.getComponent<MiniText>().drawWithBackground(dstRect.x, dstRect.y, imgW, imgH, tmp);

        //cập nhật tên gõ vào
        auto& wN = writeName.getComponent<MiniText>();
        wN.SetLabelText("Enter your name:", "Type_name");   
        wN.setPosition(dstRect.x + (dstRect.w - wN.getWidth()) / 2, dstRect.y + 230); // căn giữa 
        wN.draw();
        
        auto& wN2 = writeName2.getComponent<MiniText>();
        wN2.SetLabelText(playerName + "_", "Type_name");   
        wN2.setPosition(dstRect.x + (dstRect.w - wN2.getWidth()) / 2, dstRect.y + 250);
        wN2.draw();

    }

    if (LeaderBoardButtonUp && !exitLeaderBoardUp) {
        SDL_Texture* tmp = assets->GetTexture("leaderboard");
        if (exitLeaderBoardDown) {
            tmp = assets->GetTexture("leaderboard_clicked");
        }

        int imgW = 300, imgH = 325;
        SDL_Rect dstRect = {(WIDTH - imgW) / 2, (HEIGHT - imgH) / 2, imgW, imgH};

        SDL_RenderCopy(renderer, tmp, NULL, &dstRect); // vẽ UI cho danh sách điểm
        auto& wL = writeLeaderBoard.getComponent<MiniText>();

        vector<Player> p = leaderBoard->getPlayers();
        int y = dstRect.y + 91;
        for (size_t i = 0; i < p.size(); ++i) {
            wL.SetLabelText(p[i].name + ": " + to_string(p[i].scores), "Type_name");
            wL.setPosition(dstRect.x + (dstRect.w - wL.getWidth()) / 2, y);
            wL.draw();
            y += 24;
        }

    }

    if (isPausedUp) {
        //vẽ một khung bán trong suốt
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect  blur = {0,0, WIDTH, HEIGHT};
        SDL_RenderFillRect(renderer, &blur);

        //căn giữa 
        SDL_Texture* pauseTex = assets->GetTexture("pause_screen");
        int texW, texH;
        //lấy kích thước của texture
        SDL_QueryTexture(pauseTex, NULL, NULL, &texW, &texH);
        SDL_Rect dst{(WIDTH  - texW) / 2, (HEIGHT - texH) / 2, texW, texH};

        auto& ps = PauseScreen->getComponent<TransformComponent>();
        ps.position.x = screen.x + dst.x;
        ps.position.y = screen.y + dst.y;
        ps.width = dst.w;
        ps.height = dst.h;

        // cout << ps.position.x << " " << ps.position.y << endl;

        //Vẽ thẳng lên renderer
        SDL_RenderCopy(renderer, pauseTex, NULL, &dst);

    }

    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    // reset các trạng thái cần thiết
    UIwriteName = false;
    playButtonClickedUp = false;
    exitGameloseUp = false;
    isLogoActive = true;
    isTypingName = false;
    playerName = ""; // reset tên người chơi
    SDL_StopTextInput(); // dừng nhập tên
    leaderBoard->loadFromFile(); // load lai du lieu tu file leaderboard.txt 
    LeaderBoardButtonUp = false;
    exitLeaderBoardUp = false;
 
    // reset vị trí của logo
    logoPositionX = (1024 - 200) / 2;
    if (logo.hasComponent<TransformComponent>()) {
        logo.getComponent<TransformComponent>().position.x = logoPositionX;
        logo.getComponent<TransformComponent>().position.y = 700;
        logo.getComponent<TransformComponent>().velocity.Zero();
    }
 
    // phát lại nhạc
    assets->pauseMusic();
    assets->playMusic("thememusic", -1);
}

void Game::quit(){
    assets->quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "quit game";
}
