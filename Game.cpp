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
ScoreSystem* scoreSystem = new ScoreSystem(); // 27

bool Game::isSquashed = false; //26
Uint32 squashStartTime = 0; // 25

bool Game::playButtonClickedUp = false; // 28
bool Game::playButtonClickedDown = false; //32
Entity* Game::playButton = NULL;// 28
int logoPositionX = 420;// 29

bool Game::UIwriteName = false;// 30
bool Game::isLogoActive = true; // 31

bool Game::exitGameloseUp = false; // 32
bool Game::exitGameloseDown = false; // 32

AssetManager* Game::assets = new AssetManager(&manager);//19 + 23

bool Game::isRunning = false;// 17
 
Entity& player = manager.addEntity();//6
Entity& label = manager.addEntity(); //25
Entity& logo = manager.addEntity(); //28
Entity& writeName = manager.addEntity(); //30
// vector<Entity*> cars;

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

    //thêm âm thanh
    assets->loadMusic("thememusic", "assets/sound/thememusic.mp3");
    assets->loadSound("chickensound","assets/sound/chicken_sound.mp3");
    assets->loadSound("crashsound", "assets/sound/chicken_crashsound.mp3");

    //thêm font chữ
    assets->loadFont("font", "assets/fonts/vgafix.fon", 30);
    label.addComponent<MiniText>(10, 10, "", "font", black);

    //thêm logo và nút chơi
    assets->AddTexture("logo", "assets/images/logo.png");
    logo.addComponent<TransformComponent>(logoPositionX, 700, 200, 92, 1, 0, 0);
    logo.addComponent<SpriteComponent>("logo");

    assets->AddTexture("play_button", "assets/images/Play_Button.png");
    assets->AddTexture("play_button_clicked", "assets/images/PLay_Button_Clicked.png");
    playButton = &manager.addEntity(); //28
    playButton->addComponent<TransformComponent>(460, 970, 100, 41, 1, 0, 0); 
    playButton->addComponent<SpriteComponent>("play_button");

 

    writeName.addComponent<MiniText>(256, 256,"Your name is : ", "font", white);

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
    
    SDL_PollEvent(&event); 
    
}

void Game::update(){
    
    // chỉ update cho xe chạy nếu chưa nhấn nút chơi
    if (!playButtonClickedUp) {
        manager.refresh();//8
        manager.update();//6 
        return;
    } 

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

    // // Cập nhật vị trí của toa đầu tiên
    // cars[0]->getComponent<TransformComponent>().position.x += cars[0]->getComponent<TransformComponent>().velocity.x;
    // cars[0]->getComponent<TransformComponent>().position.y += cars[0]->getComponent<TransformComponent>().velocity.y;

    // // Cập nhật vị trí các toa sau nối đuôi toa trước
    // for (size_t i = 1; i < cars.size(); ++i) {
    //     TransformComponent& prevTransform = cars[i - 1]->getComponent<TransformComponent>();
    //     TransformComponent& currTransform = cars[i]->getComponent<TransformComponent>();

    // // Di chuyển toa hiện tại theo toa trước
    // currTransform.position.x = prevTransform.position.x - prevTransform.width - 100;
    // currTransform.position.y = prevTransform.position.y;
    // }

    //ghi điểm số 
    scoreSystem->updateScore(player.getComponent<TransformComponent>().position.y);
    label.getComponent<MiniText>().SetLabelText(scoreSystem->getScoreText(), "font");

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
        }
        return; // Không xử lý logic khác khi đang ở trạng thái "bị bẹp"
    }

    for(auto& d : dangers){
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, d->getComponent<ColliderComponent>().collider)){
            player.getComponent<TransformComponent>().velocity.Zero();
            assets->playSound("crashsound", 0);
            player.getComponent<TransformComponent>().position = {512, 970};
        }
    }

    // if (UIwriteName) {
    //     for (auto& v : vehicles) {
    //         v->getComponent<TransformComponent>().velocity.Zero();
    //     }
    // }

    // cout << "chicken position: (" 
    // << player.getComponent<TransformComponent>().position.x << ", " 
    // << player.getComponent<TransformComponent>().position.y << ")" << std::endl;

}

void Game::render(){   
    SDL_RenderClear(renderer); // ve map trc roi moi den nhan vat//5

    for (auto& t : tiles) t->draw();
    //for (auto& car : cars) car->draw();
    //for (auto& c : colliders) c->draw();

    for (auto& v : vehicles) v->draw();
    for (auto& d : dangers) d->draw();
    for (auto& p : players) p->draw(); 
    
    if (playButtonClickedUp) label.draw(); // Score text
    
    if (isLogoActive) {
        logo.draw();
    }
    
    // vẽ button khi chưa thả nút ra  
    if (!playButtonClickedUp) {
        if (playButtonClickedDown) { // nếu nút được bấm
            playButton->getComponent<SpriteComponent>().setTex("play_button_clicked");
            playButton->draw();
        }
        else { // nút khi chưa được bấm
            playButton->getComponent<SpriteComponent>().setTex("play_button");
            playButton->draw();
        }
    }

    if (UIwriteName && !exitGameloseUp) {


        SDL_Texture* tmp = assets->GetTexture("losing_screen");
        if (exitGameloseDown) {
            tmp = assets->GetTexture("losing_screen_clicked");
        }
        
        int imgW = 300, imgH = 325;
        SDL_Rect dstRect;
        dstRect.x = (WIDTH - imgW) / 2;
        dstRect.y = (HEIGHT - imgH) / 2;

        writeName.getComponent<MiniText>().drawWithBackground(dstRect.x, dstRect.y, imgW, imgH, tmp);
    }

    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    // reset các trạng thái cần thiết
    UIwriteName = false;
    playButtonClickedUp = false;
    exitGameloseUp = false;
    isLogoActive = true;
 
    // reset vị trí của logo
    logoPositionX = 420;
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
    if (scoreSystem){
        scoreSystem->saveHighScore();
    }
    assets->quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    cout << "quit game";
}
