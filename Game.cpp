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
ScoreSystem* scoreSystem; // 27

bool Game::isSquashed = false; //26
Uint32 squashStartTime = 0; // 25

AssetManager* Game::assets = new AssetManager(&manager);//19 + 23

bool Game::isRunning = false;// 17
 
Entity& player = manager.addEntity();//6
Entity& label = manager.addEntity(); //25
// vector<Entity*> cars;

vector<Entity*>& tiles = manager.getGroup(Game::groupMap); // tiles là một vector các entity trong nhóm groupMap
vector<Entity*>& players = manager.getGroup(Game::groupPlayer);
vector<Entity*>& colliders = manager.getGroup(Game::groupColliders);//18
vector<Entity*>& dangers = manager.getGroup(Game::groupDangers);//21
vector<Entity*>& vehicles = manager.getGroup(Game::groupVehicles); // 26

Game::Game(){
    scoreSystem = new ScoreSystem();
}

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

    //thêm âm thanh
    assets->loadMusic("thememusic", "assets/sound/thememusic.mp3");
    assets->loadSound("chickensound","assets/sound/chicken_sound.mp3");
    assets->loadSound("crashsound", "assets/sound/chicken_crashsound.mp3");

    //thêm font chữ
    assets->loadFont("font", "assets/fonts/vgafix.fon", 30);
    label.addComponent<MiniText>(10, 10, "", "font", black);

    //vẽ map
    gameMap = new Map("terrain", 1, 32);
    gameMap->LoadMap("assets/maptile/map1.map", 64, 32, 8); //14 x = 64 vì trong map1.map có 64 dòng 

    
    //vẽ nhân vật
    player.addComponent<TransformComponent>(512, 970, 24, 20, 1, 0, 0);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);

    //thêm hệ thống điểm
    scoreSystem->setLastPlayerY(player.getComponent<TransformComponent>().position.y); // lưu vị trí cuối của player để so sánh với vị trí hiện tại

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

    //viết chữ
    //label.getComponent<MiniText>().SetLabelText();
    //lastPlayerY = player.getComponent<TransformComponent>().position.y; // lưu vị trí cuối của player để so sánh với vị trí hiện tại

}

void Game::handleEvents(){

    if (isSquashed) return;
    
    SDL_PollEvent(&event);   
    
}

void Game::update(){

    //mapcollider
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider; // lấy collider của player
    Vector2D playerPos = player.getComponent<TransformComponent>().position; // lấy vị trí của player để khi va chạm với vật cản thì trở về trạng thái chưa va chạm 

    int currentPlayerY = playerPos.y; // lấy vị trí hiện tại của player

    manager.refresh();//8
    manager.update();//6 

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
    scoreSystem->updateScore(currentPlayerY); // cập nhật điểm số dựa trên vị trí hiện tại của player
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
    label.draw();

    SDL_RenderPresent(renderer);
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
