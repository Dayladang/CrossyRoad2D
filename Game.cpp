#include "Game.h"
#include "Map.cpp"//5
#include "ECS/Components.h"
#include "Vector2D.cpp"//8
#include "Collision.cpp"//10
#include "ECS/ECS.cpp" // 16

using namespace std;

Map* gameMap;
Manager manager;//6

SDL_Renderer* Game::renderer = NULL; //5
SDL_Event Game::event;//9

bool Game::isRunning = false;// 17
 
Entity& player = manager.addEntity();//6
Entity& car = manager.addEntity();//6
Entity& car2 = manager.addEntity();
Entity& car3 = manager.addEntity();
Entity& car4 = manager.addEntity();
Entity& train = manager.addEntity();

// Entity& wall1 = manager.addEntity(); //10
// Entity& wall2 = manager.addEntity(); //12

SDL_Rect Game::screen = {0, 0, WIDTH, HEIGHT}; //17

vector<Entity*>& tiles = manager.getGroup(Game::groupMap); // tiles là một vector các entity trong nhóm groupMap
vector<Entity*>& players = manager.getGroup(Game::groupPlayer);
vector<Entity*>& colliders = manager.getGroup(Game::groupColliders);//18

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

    isRunning = true;

    gameMap = new Map("imgs/color.png", 1, 32);
    gameMap->LoadMap("imgs/map.map", 64, 32, 8); //14 

    player.addComponent<TransformComponent>(512, 990, 24, 20, 1, 0, 0);
    player.addComponent<SpriteComponent>("imgs/chick_total.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);

    car.addComponent<TransformComponent>(1024 + 100, 760, 75, 44, 1, -6, 0);
    car.addComponent<SpriteComponent>("imgs/taxi.png");
    car.addComponent<ColliderComponent>("car");
    car.addGroup(groupMap);

    car2.addComponent<TransformComponent>(-100, 795, 84, 43, 1, 3, 0);  
    car2.addComponent<SpriteComponent>("imgs/truck.png");
    car2.addComponent<ColliderComponent>("truck");
    car2.addGroup(groupMap);

    car3.addComponent<TransformComponent>(1024 + 100, 840, 88, 47, 1, -4, 0);  
    car3.addComponent<SpriteComponent>("imgs/redtruck.png");
    car3.addComponent<ColliderComponent>("redtruck");
    car3.addGroup(groupMap);

    car4.addComponent<TransformComponent>(-100, 880, 80, 44, 1, 6, 0);  
    car4.addComponent<SpriteComponent>("imgs/ambulance.png");
    car4.addComponent<ColliderComponent>("ambulance");
    car4.addGroup(groupMap);

    train.addComponent<TransformComponent>(-100, 200, 92, 41, 1, 8, 0);  
    train.addComponent<SpriteComponent>("imgs/train.png");
    train.addComponent<ColliderComponent>("train");
    train.addGroup(groupMap);

}

void Game::handleEvents(){
   
    SDL_PollEvent(&event);
    
}

void Game::update(){

    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider; // lấy collider của player
    Vector2D playerPos = player.getComponent<TransformComponent>().position; // lấy vị trí của player để khi va chạm với vật cản thì trở về trạng thái chưa va chạm 

    manager.refresh();//8
    manager.update();//6 

    for (auto& c : colliders){
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider; // lấy collider của vật cản 

        if (Collision::AABB(cCol, playerCol)){
            player.getComponent<TransformComponent>().position = playerPos; // trở lại vị trí trc khi va chạm
        }
    }

    screen.x = player.getComponent<TransformComponent>().position.x - WIDTH / 2; // làm cho screen di chuyển theo player
    screen.y = player.getComponent<TransformComponent>().position.y - HEIGHT / 2; 

    if (screen.x < 0) screen.x = 0; // giữ cho screen không di chuyển ra ngoài map
    if (screen.y < 0) screen.y = 0;
    if (screen.x > screen.w) screen.x = screen.w;
    if (screen.y > screen.h) screen.y = screen.h;

    if(Collision::AABB(player.getComponent<ColliderComponent>().collider, car.getComponent<ColliderComponent>().collider)){
        player.getComponent<TransformComponent>().velocity.Zero();       
        cout << "đâm rồi thằng ngu" << endl;
        SDL_Delay(5000);
        isRunning = false;
    }

    if(Collision::AABB(player.getComponent<ColliderComponent>().collider, car2.getComponent<ColliderComponent>().collider)){
        player.getComponent<TransformComponent>().velocity.Zero();
        cout << "đâm rồi thằng ngu" << endl;
        SDL_Delay(5000);
        isRunning = false;
    }

    // cout << "chicken position: (" 
    // << player.getComponent<TransformComponent>().position.x << ", " 
    // << player.getComponent<TransformComponent>().position.y << ")" << std::endl;

    // for (ColliderComponent* c : colliders){
    //     Collision::AABB(player.getComponent<ColliderComponent>(), *c);
    // }
}

void Game::render(){   
    SDL_RenderClear(renderer); // ve map trc roi moi den nhan vat//5

    for (auto& t : tiles) t->draw();
    for (auto& c : colliders) c->draw();
    for (auto& p : players) p->draw();

    SDL_RenderPresent(renderer);
}

void Game::quit(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "quit game";
}
