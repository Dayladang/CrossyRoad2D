#include "Game.h"
#include "Map.h"//5
#include "ECS/Components.h"
#include "Vector2D.h"//8
#include "Collision.h"//10
#include "ECS/ECS.h" // 16
#include "ButtonManager.h"

using namespace std;

const char WINDOW_TITLE[] = " Crossy Road 2D ";

Map* gameMap;
Manager manager;//6

SDL_Renderer* Game::renderer = NULL; //5
SDL_Event Game::event;//9

ButtonManager* Button = new ButtonManager(); // 43

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

bool ButtonManager::playButtonClickedUp = false; // 28
bool ButtonManager::playButtonClickedDown = false; //32
Entity* ButtonManager::playButton = NULL;// 28
int logoPositionX = (1024 - 200) / 2 ; //29

bool Game::isLogoActive = true; // 31

bool ButtonManager::exitGameloseUp = false; // 32
bool ButtonManager::exitGameloseDown = false; // 32

bool Game::UIwriteName = false;// 30
string Game::playerName = ""; // 34
bool Game::isTypingName = false; // 34

bool ButtonManager::LeaderBoardButtonUp = false;// 37
bool ButtonManager::LeaderBoardButtonDown = false; // 37
bool ButtonManager::exitLeaderBoardUp = false;// 34
bool ButtonManager::exitLeaderBoardDown = false; // 37
Entity* ButtonManager::LeaderBoardButton = NULL; // 37

bool ButtonManager::isPausedUp = false; // 38
bool ButtonManager::isPausedDown = false; //38
Entity* ButtonManager::PauseButton = NULL; //38
Entity* ButtonManager::PauseScreen = NULL; //38

Entity* ButtonManager::quitGame = NULL; //39
bool ButtonManager::quitGameUp = false;//39
bool ButtonManager::quitGameYesUp = false;//39
bool ButtonManager::quitGameYesDown = false; //39
bool ButtonManager::quitGameNoUp = false;//39
bool ButtonManager::quitGameNoDown = false;// 39

Entity* ButtonManager::mutedButton = NULL; // 40
bool ButtonManager::unMutedButtonUp = true; //40
bool ButtonManager::unMutedButtonDown = false; //40
bool ButtonManager::MutedButtonUp = false; //40
bool ButtonManager::MutedButtonDown = false; //40

vector<string> Game::mapList = {"assets/maptile/map1.map", "assets/maptile/map2.map", "assets/maptile/map3.map"};
vector<string> Game::vehiclesFiles = {"assets/maptile/vehicles.txt", "assets/maptile/vehicles2.txt", "assets/maptile/vehicles3.txt"};
int Game::currentMapIndex = 0; // Map đang chơi 41
int lastCurrentMapIndex = 0; // 41
int Game::Mapcounter = 0; // 41
bool Game::isMap2Loading = false;// 42
bool Game::resetDone = false;// 42
int maxY = 0; // giới hạn của camera 42

Entity* Game::exitWriteName = NULL; // 42

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
    assets->loadSound("click_up", "assets/sound/click_up.mp3");
    assets->loadSound("click_down", "assets/sound/click_down.mp3");

    //thêm font chữ
    assets->loadFont("font", "assets/fonts/pixelfont.ttf", 15);
    assets->loadFont("gameover_font", "assets/fonts/pixelfont.ttf", 40);
    assets->loadFont("Type_name", "assets/fonts/pixelfont.ttf", 12);
    scoreBoard.addComponent<MiniText>(10, 10, "", "font", black);

    //thêm logo
    assets->AddTexture("logo", "assets/images/logo.png");
    logo.addComponent<TransformComponent>(logoPositionX, 700, 200, 92, 1, 0, 0);
    logo.addComponent<SpriteComponent>("logo");

    //thêm các nút chơi
    Button->initButtons();

    //các khung viết chữ
    exitWriteName = &manager.addEntity();
    exitWriteName->addComponent<TransformComponent>(0, 0, 256, 256, 1, 0, 0);
    writeScore.addComponent<MiniText>(256, 256, "", "gameover_font", white);
    writeName.addComponent<MiniText>(0, 0, "", "Type_name", white);
    writeName2.addComponent<MiniText>(0, 0, "", "Type_name", white);
    writeLeaderBoard.addComponent<MiniText>(0, 0, "", "Type_name", white);

    //vẽ map
    gameMap = new Map("terrain", 1, 32);
    gameMap->LoadMap(mapList[2], 64, 32, 8, 0); //14 x = 64 vì trong map1.map có 64 dòng 

    
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

    gameMap->loadVehiclesForMap(2);

    //phát âm thanh
    assets->playMusic("thememusic", -1);

}

void Game::handleEvents(){

    if (isSquashed) return;
    
    if (!ButtonManager::isPausedUp) SDL_PollEvent(&event); // nếu không ở trạng thái pause thì cập nhật bình thường 
    
    else { // nếu ở trạng thái pause thì vì return luôn, không cho manager cập nhật nên phải dùng một biến event khác để bắt sự kiện nếu chuột bấm vào PauseScreen
        SDL_Event e;
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                isRunning = false;
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {

                int ngumouseX, ngumouseY;
                SDL_GetMouseState(&ngumouseX, &ngumouseY); // đây là lấy tọa độ chuột trong của sổ

                int mouseX = ngumouseX + screen.x;// lấy tọa độ chuột trong map
                int mouseY = ngumouseY + screen.y;

                if (mouseX >= ButtonManager::PauseScreen->getComponent<TransformComponent>().position.x &&
                    mouseX <= ButtonManager::PauseScreen->getComponent<TransformComponent>().position.x + ButtonManager::PauseScreen->getComponent<TransformComponent>().width &&
                    mouseY >= ButtonManager::PauseScreen->getComponent<TransformComponent>().position.y &&
                    mouseY <= ButtonManager::PauseScreen->getComponent<TransformComponent>().position.y + ButtonManager::PauseScreen->getComponent<TransformComponent>().height) {

                        ButtonManager::isPausedUp = false;
                        assets->playSound("click_down", 0);

                }
            }

            event = e; // cập nhật event chính khi game đang pause
        }
    }
   
}

void Game::update(){

    // chỉ update cho xe chạy nếu chưa nhấn nút chơi
    if (!ButtonManager::playButtonClickedUp) {

        manager.refresh();//8
        manager.update();//6 

        if (ButtonManager::quitGameUp) { // xử lí quitGame kể cả khi nút chơi chưa được ấn
            if (ButtonManager::quitGameYesUp) isRunning = false;
            else if (ButtonManager::quitGameNoUp) {
                ButtonManager::quitGameUp    = false;
                ButtonManager::quitGameNoUp  = false;
            }
        }

        if (ButtonManager::MutedButtonUp) { // thêm điều kiện này để có thể mute nhạc ngay khi chưa ấn play
            assets->MuteMusicAndSound();
        }
        else if (ButtonManager::unMutedButtonUp) {
            assets->unMuteMusicAndSound();
        }

        return;

    } 

    // nếu ấn PauseButton thì tạm thời không cập nhật
    if (ButtonManager::isPausedUp){
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
        if (ButtonManager::playButtonClickedUp) {
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
    for (auto c : colliders){
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider; // lấy collider của vật cản 

        if (Collision::AABB(cCol, playerCol)){
            player.getComponent<TransformComponent>().position = playerPos; // trở lại vị trí trc khi va chạm
        }
    }

    //camera
    screen.x = player.getComponent<TransformComponent>().position.x - WIDTH / 2; // làm cho screen di chuyển theo player
    screen.y = player.getComponent<TransformComponent>().position.y - HEIGHT / 2; 

    // Khai báo chiều cao thực tế của từng map
    int map1Height = 512;
    int map2Height = 1024; // Giả sử map2 cao 1024px, điều chỉnh theo kích thước thực tế

    // Tính toán giới hạn dựa trên map hiện tại
    if (currentMapIndex == 0) {
        maxY = map1Height;
    } else if (currentMapIndex == 1) {
        maxY = -map2Height * (Mapcounter - 1); // các Map mới sẽ có chiều cao âm
    }

    if (screen.x < 0) screen.x = 0; // giữ cho screen không di chuyển ra ngoài map
    if (screen.x > screen.w) screen.x = screen.w;
    if (screen.y > maxY) screen.y = maxY;

    //ghi điểm số 
    scoreSystem->updateScore(player.getComponent<TransformComponent>().position.y);
    scoreBoard.getComponent<MiniText>().SetLabelText(scoreSystem->getScoreText(), "font");

    // bắt sự kiện va chạm
    for(auto v : vehicles){
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
            currentMapIndex = 0;
            screen.y = 512;
            // cout << manager.getGroup(groupMap).size() << endl;
            UIwriteName = true;
            isSquashed = false;
            currentScore = scoreSystem->getScore();
            isTypingName = true;
            SDL_StartTextInput(); 
        }
        return; // Không xử lý logic khác khi đang ở trạng thái "bị bẹp"
    }

    for(auto d : dangers){
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, d->getComponent<ColliderComponent>().collider)){
            player.getComponent<TransformComponent>().velocity.Zero();
            assets->playSound("crashsound", 0);
            player.getComponent<TransformComponent>().position = {512, 970};
            currentMapIndex = 0;
            screen.y = 512;
            UIwriteName = true;
            currentScore = scoreSystem->getScore();
            isTypingName = true;
            SDL_StartTextInput();
        }
    }

    if (ButtonManager::quitGameYesUp) isRunning = false;

    if (ButtonManager::quitGameNoUp) {
        ButtonManager::quitGameUp = false;
        ButtonManager::quitGameNoUp = false;
    }

    // Khi phát hiện người chơi đến cuối map
    int playerY = player.getComponent<TransformComponent>().position.y;
    // Tính mapEndY dựa trên map hiện tại
    int mapEndY; // Tọa độ cuối của map hiện tại
    if (currentMapIndex == 0) {
        mapEndY = 0; // Cuối map1 là y=0
    } else {
        mapEndY = Mapcounter * (-1024); // Cuối các map khác
    }

    int threshold = 250; // Khoảng cách để bắt đầu load map mới
    if (abs(playerY - mapEndY) < threshold && !isMap2Loading) {
    
        if (!isMap2Loading) {
            isMap2Loading = true;

            Mapcounter++;
            
            // Chọn ngẫu nhiên chỉ số map
            while (currentMapIndex == lastCurrentMapIndex) {
                currentMapIndex = ( rand() % (mapList.size() - 1) ) + 1; // chọn ngẫu nhiên chỉ số map khác với số trước đó bắt đầu từ 1 đến size() - 1
            }
            lastCurrentMapIndex = currentMapIndex;
            
            // Load map mới
            int offsetY = -Mapcounter * 1024; // offset cho map mới
            gameMap->LoadMap(mapList[currentMapIndex], 64, 32, 8, offsetY);
            
            gameMap->loadVehiclesForMap(currentMapIndex);

            // Cập nhật các group
            colliders = manager.getGroup(groupColliders);
            vehicles = manager.getGroup(groupVehicles);
            dangers = manager.getGroup(groupDangers);
            tiles = manager.getGroup(groupMap);

            // for (auto d : dangers) {
            //     if (d && d->hasComponent<TransformComponent>())
            //         std::cout << "Danger y: " << d->getComponent<TransformComponent>().position.y << std::endl;
            // }

            // Quan trọng: Cập nhật ngay lập tức ràng buộc camera cho map2
            int minY = -map2Height;
            int maxY = 0;

            // Cập nhật camera để phù hợp với vị trí mới
            screen.y = player.getComponent<TransformComponent>().position.y - HEIGHT / 2;
            
            cout << "Map " << currentMapIndex << " loaded! Player at y = " << player.getComponent<TransformComponent>().position.y << endl;
        }

        isMap2Loading = false;
    } 

    // Khi phát hiện người chơi đã di chuyển hoàn toàn vào map2
    if (screen.y < -HEIGHT - 200 && !resetDone) {
        resetDone = true;
        isMap2Loading = false;
        
        // cout << "Shifting map1 above map2..." << endl;

        // THÊM: Kiểm tra nếu tọa độ âm quá lớn (ví dụ dưới -10000), reset lại hệ tọa độ
        if (player.getComponent<TransformComponent>().position.y < -10000) {
            cout << "Resetting coordinate system..." << endl;
            
            // Lưu vị trí tương đối của player trong map hiện tại
            float relativeY = fmod(player.getComponent<TransformComponent>().position.y, map2Height);
            if (relativeY > 0) relativeY -= map2Height; // Đảm bảo giữ tọa độ âm
            
            // Load lại map2 với offset mới
            // int offsetY = -Mapcounter * 1024;
            // gameMap->LoadMap(mapList[currentMapIndex], 64, 32, 8, offsetY);
            
            // Đặt lại vị trí player về tọa độ tương đối
            player.getComponent<TransformComponent>().position.y = relativeY;
            
            return; // Bỏ qua phần dịch chuyển map1
        }
        
        // XÓA TOÀN BỘ ENTITY THUỘC MAP1 (y >= 0)
        set<Entity*> uniqueEntities;

        for (auto t : tiles)
            if (t->hasComponent<TransformComponent>() && t->getComponent<TransformComponent>().position.y >= 0)
                uniqueEntities.insert(t);
        for (auto v : vehicles)
            if (v->hasComponent<TransformComponent>() && v->getComponent<TransformComponent>().position.y >= 0)
                uniqueEntities.insert(v);
        for (auto c : colliders)
            if (c->hasComponent<TransformComponent>() && c->getComponent<TransformComponent>().position.y >= 0)
                uniqueEntities.insert(c);
        for (auto d : dangers)
            if (d->hasComponent<TransformComponent>() && d->getComponent<TransformComponent>().position.y >= 0)
                uniqueEntities.insert(d);

        vector<Entity*> entitiesToDestroy(uniqueEntities.begin(), uniqueEntities.end());

        // cout << "Entities to destroy: " << entitiesToDestroy.size() << endl;

        for (auto e : entitiesToDestroy) {
            e->destroy();
            // cout << "Entity " << e << " active after destroy: " << e->isActive() << endl;
        }

        manager.refresh();
        entitiesToDestroy.clear();
        uniqueEntities.clear();

        //load lại map vì vừa mới xóa 
        int offsetY = -Mapcounter * 1024; // offset cho map mới 
        gameMap->LoadMap(mapList[currentMapIndex], 64, 32, 8, offsetY); // Load lại map2 với offset mới

        // cập nhật các group
        tiles = manager.getGroup(groupMap);
        vehicles = manager.getGroup(groupVehicles);
        colliders = manager.getGroup(groupColliders);
        dangers = manager.getGroup(groupDangers);

        // cout << "Map1 shifted above map2! Player at y = " << player.getComponent<TransformComponent>().position.y << endl;
    }

    // cout << exitWriteName->getComponent<TransformComponent>().position.x << " "
    //     << exitWriteName->getComponent<TransformComponent>().position.y << endl;

    cout << "chicken position: (" 
    << player.getComponent<TransformComponent>().position.x << ", " 
    << player.getComponent<TransformComponent>().position.y << ")" << endl;

    // Debug để kiểm tra currentMapIndex
    // cout << "Player pos: " << player.getComponent<TransformComponent>().position.y 
    //     << ", Camera pos: " << screen.y 
    //     << ", maxY: " << maxY 
    //     << ", MapIndex: " << currentMapIndex << endl;

}

void Game::render(){  
    try{ 
    SDL_RenderClear(renderer); // ve map trc roi moi den nhan vat//5

    for (auto t : tiles) {
        if (t && t->isActive()) {  // Kiểm tra an toàn
            t->draw();
        }
    }
    //for (auto& c : colliders) c->draw();

    for (auto v : vehicles){
        if (v && v->isActive()) {  // Kiểm tra an toàn
            v->draw();
        }
    }
    for (auto d : dangers) {
        if (d && d->isActive()) {  // Kiểm tra an toàn
            d->draw();
        }
    }
    for (auto p : players) {
        if (p && p->isActive()) {  // Kiểm tra an toàn
            p->draw();
        }
    }
    
    if (ButtonManager::playButtonClickedUp) { 
        scoreBoard.draw(); // Score text
    }
    
    if (isLogoActive) {
        logo.draw();
    }
    
    // Vẽ các nút chơi
    Button->drawButtons();

    if (UIwriteName && !ButtonManager::exitGameloseUp) {

        SDL_Texture* tmp = assets->GetTexture("losing_screen");
        if (ButtonManager::exitGameloseDown) {
            tmp = assets->GetTexture("losing_screen_clicked");
        }
        
        int imgW = 300, imgH = 325;
        SDL_Rect dstRect = {(WIDTH - imgW) / 2, (HEIGHT - imgH) / 2, imgW, imgH};

        // Cập nhật vị trí của exitWriteName entity
        auto& exitPos = exitWriteName->getComponent<TransformComponent>();
        exitPos.position.x = screen.x + dstRect.x;
        exitPos.position.y = screen.y + dstRect.y;
        exitPos.width = dstRect.w;
        exitPos.height = dstRect.h;

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

    if (ButtonManager::LeaderBoardButtonUp && !ButtonManager::exitLeaderBoardUp) {
        SDL_Texture* tmp = assets->GetTexture("leaderboard");
        if (ButtonManager::exitLeaderBoardDown) {
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
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "Unknown exception during render!" << endl;
    }

    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    // reset các trạng thái cần thiết
    Button->resetButtons();
    UIwriteName = false;
    isLogoActive = true;
    isTypingName = false;
    playerName = ""; // reset tên người chơi
    SDL_StopTextInput(); // dừng nhập tên
    leaderBoard->loadFromFile(); // load lai du lieu tu file leaderboard.txt 
    lastCurrentMapIndex = currentMapIndex = 0;
    isMap2Loading = false;
    resetDone = false;
    Mapcounter = 0;
    maxY = 512;

    try {
        // load lại map1
        set<Entity*> uniqueEntities;

        // cout << "Before destroy - Tiles count: " << tiles.size() << endl;
            
        // Thu thập, không hủy ngay
        for (auto t : tiles) uniqueEntities.insert(t);
        for (auto v : vehicles) uniqueEntities.insert(v);
        for (auto c : colliders) uniqueEntities.insert(c);
        for (auto d : dangers) uniqueEntities.insert(d);

        vector<Entity*> entitiesToDestroy(uniqueEntities.begin(), uniqueEntities.end());
            
        for (auto e : entitiesToDestroy) {
            if (
                e != &player &&
                e != &logo &&
                e != ButtonManager::playButton &&
                e != ButtonManager::PauseButton &&
                e != ButtonManager::LeaderBoardButton &&
                e != ButtonManager::mutedButton &&
                e != exitWriteName &&
                e != ButtonManager::quitGame &&
                e->isActive()
            ) {
                e->destroy();
            }
        }   

        manager.refresh();

        entitiesToDestroy.clear();
        uniqueEntities.clear();
        tiles.clear(); // <-- QUAN TRỌNG
        vehicles.clear();
        colliders.clear();
        dangers.clear();

        // cout << "After refresh - Tiles count: " << manager.getGroup(groupMap).size() << endl;

        gameMap->LoadMap(mapList[currentMapIndex], 64, 32, 8, 0);

        gameMap->loadVehiclesForMap(currentMapIndex);

        // cout << "After load - Tiles count: " << manager.getGroup(groupMap).size() << endl;
       
        // cập nhật lại các group
        tiles = manager.getGroup(groupMap);
        vehicles = manager.getGroup(groupVehicles);
        colliders = manager.getGroup(groupColliders);
        dangers = manager.getGroup(groupDangers);

        player.getComponent<TransformComponent>().position = {512, 970};
        
        // reset vị trí camera
        screen.x = player.getComponent<TransformComponent>().position.x - WIDTH / 2; // làm cho screen di chuyển theo player
        screen.y = player.getComponent<TransformComponent>().position.y - HEIGHT / 2;

        // cout << "screen placed at : " << screen.x << " " << screen.y << endl;
    
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
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "Unknown exception during reset!" << endl;
    }
}

void Game::quit(){
    assets->quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "quit game";
}
