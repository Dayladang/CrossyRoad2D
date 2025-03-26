#include "Game.h"
#include "Map.cpp"//5
#include "ECS/Components.h"
#include "Vector2D.cpp"//8
#include "Collision.cpp"//10
#include "ECS/ECS.cpp" // 16

using namespace std;

Manager manager;//6

SDL_Renderer* Game::renderer = NULL; //5
SDL_Event Game::event;//9

vector<ColliderComponent*> Game::colliders; // 12
 
Entity& player = manager.addEntity();//6
Entity& car = manager.addEntity();//6
Entity& car2 = manager.addEntity();

// Entity& wall1 = manager.addEntity(); //10
// Entity& wall2 = manager.addEntity(); //12

enum groupLabels : size_t { // size_t được định nghĩa trong ECS.h là Group
    groupMap,
    groupPlayer,
    groupColliders
};

const char* mapfile = "C:\\Users\\ADMIN\\OneDrive\\Desktop\\git_exercise\\imgs\\color.png"; // 14

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

    Map::LoadMap("C:\\Users\\ADMIN\\OneDrive\\Desktop\\git_exercise\\imgs\\map.map", 32, 32, 8); //14 

    player.addComponent<TransformComponent>(WIDTH / 2, 580, 24, 24, 1, 0, 0);
    player.addComponent<SpriteComponent>("imgs/chick_total.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);

    car.addComponent<TransformComponent>(WIDTH + 100, 370, 75, 44, 1, -6, 0);
    car.addComponent<SpriteComponent>("imgs/taxi.png");
    car.addComponent<ColliderComponent>("car");
    car.addGroup(groupMap);

    car2.addComponent<TransformComponent>(-100, 430, 84, 43, 1, 3, 0);  
    car2.addComponent<SpriteComponent>("imgs/truck.png");
    car2.addComponent<ColliderComponent>("truck");
    car2.addGroup(groupMap);

}

void Game::handleEvents(){
   
    SDL_PollEvent(&event);

        if(event.type == SDL_QUIT) isRunning = false;

        else if ( event.type == SDL_KEYDOWN ){
            const Uint8* check = SDL_GetKeyboardState(NULL);
            if(check[SDL_SCANCODE_ESCAPE]) isRunning = false;
        }
    
}

void Game::update(){
    manager.refresh();//8
    manager.update();//6 

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

    for (ColliderComponent* c : colliders){
        Collision::AABB(player.getComponent<ColliderComponent>(), *c);
    }
}

vector<Entity*>& tiles = manager.getGroup(groupMap); // tiles là một vector các entity trong nhóm groupMap
vector<Entity*>& players = manager.getGroup(groupPlayer);

void Game::render(){   
    SDL_RenderClear(renderer); // ve map trc roi moi den nhan vat//5

    for (auto& t : tiles) t->draw();
    for (auto& p : players) p->draw();

    SDL_RenderPresent(renderer);
}

void Game::quit(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "quit game";
}

void Game::AddTile(int srcX, int srcY, int xpos, int ypos){
    Entity& Map = manager.addEntity(); //13
    Map.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
    Map.addGroup(groupMap);
}
