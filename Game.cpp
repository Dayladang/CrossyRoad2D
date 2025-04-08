#include "Game.h"
#include "Map.cpp"//5
#include "ECS/Components.h"
#include "Vector2D.cpp"//8
#include "Collision.cpp"//10
#include "ECS/ECS.cpp" // 16
#include "AssetManager.cpp" //19
#include "AudioManager.cpp" // 23
#include <SDL2/SDL_ttf.h>

using namespace std;

Map* gameMap;
Manager manager;//6

SDL_Renderer* Game::renderer = NULL; //5
SDL_Event Game::event;//9

//camera 
SDL_Rect Game::screen = {0, 0, WIDTH, HEIGHT}; //17

bool Game::isSquashed = false; //26
Uint32 squashStartTime = 0; // 25

AssetManager* Game::assets = new AssetManager(&manager);
AudioManager* Game::audio = new AudioManager(); //23

bool Game::isRunning = false;// 17
 
Entity& player = manager.addEntity();//6
// vector<Entity*> cars;
Entity& train = manager.addEntity();

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

    if (audio->initAudio() == false){
        isRunning = false;
    }

    isRunning = true;
      
    //thêm nhận vật
    assets->AddTexture("terrain", "assets/color1.png");
    assets->AddTexture("player", "assets/chick_total.png");
    assets->AddTexture("taxi", "assets/taxi.png");
    assets->AddTexture("truck", "assets/truck.png");
    assets->AddTexture("redtruck", "assets/redtruck.png");
    assets->AddTexture("ambulance", "assets/ambulance.png");
    assets->AddTexture("trainleft", "assets/trainleft.png");
    assets->AddTexture("train", "assets/train.png");
    assets->AddTexture("musclecar", "assets/musclecar.png");
    assets->AddTexture("hatchback", "assets/hatchback.png");
    assets->AddTexture("luxurycar", "assets/luxurycar.png");
    assets->AddTexture("jeep", "assets/jeep.png");
    assets->AddTexture("microcar", "assets/microcar.png");
    assets->AddTexture("minivan", "assets/minivan.png");
    assets->AddTexture("SUV", "assets/SUV.png");
    assets->AddTexture("wagon", "assets/wagon.png");

    //thêm âm thanh
    audio->loadMusic("thememusic", "sound/thememusic.mp3");
    audio->loadSound("chickensound","sound/chicken_sound.mp3");
    audio->loadSound("crashsound", "sound/chicken_crashsound.mp3");

    //vẽ map
    gameMap = new Map("terrain", 1, 32);
    gameMap->LoadMap("assets/map1.map", 64, 32, 8); //14 x = 64 vì trong map1.map có 64 dòng 

    
    //vẽ nhân vật
    player.addComponent<TransformComponent>(512, 970, 24, 20, 1, 0, 0);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);

    //lấy phương tiện từ file và thêm các tính năng
    fstream file("assets/vehicles.txt", ios::in);
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
    audio->playMusic("thememusic", -1);

}

void Game::handleEvents(){

    if (isSquashed) return;
    
    SDL_PollEvent(&event);   
    
}

void Game::update(){

    //mapcollider
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider; // lấy collider của player
    Vector2D playerPos = player.getComponent<TransformComponent>().position; // lấy vị trí của player để khi va chạm với vật cản thì trở về trạng thái chưa va chạm 

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

    // bắt sự kiện va chạm
    for(auto& v : vehicles){
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, v->getComponent<ColliderComponent>().collider)){
            player.getComponent<TransformComponent>().velocity.Zero();
            audio->playSound("crashsound", 0);
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
            audio->playSound("crashsound", 0);
            player.getComponent<TransformComponent>().position = {512, 970};
        }
    }

    cout << "chicken position: (" 
    << player.getComponent<TransformComponent>().position.x << ", " 
    << player.getComponent<TransformComponent>().position.y << ")" << std::endl;

}

void Game::render(){   
    SDL_RenderClear(renderer); // ve map trc roi moi den nhan vat//5

    for (auto& t : tiles) t->draw();
    //for (auto& car : cars) car->draw();
    //for (auto& c : colliders) c->draw();
    for (auto& v : vehicles) v->draw();
    for (auto& d : dangers) d->draw();
    for (auto& p : players) p->draw();

    SDL_RenderPresent(renderer);
}

void Game::quit(){
    audio->quitAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    cout << "quit game";
}
