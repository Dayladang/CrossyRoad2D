#include "Game.h"
#include "Map.cpp"//5
#include "ECS/Components.h"
#include "Vector2D.cpp"//8
#include "Collision.cpp"//10

using namespace std;

Map* map1; //5

SDL_Renderer* Game::renderer = NULL; //5

SDL_Event Game::event;//9

Manager manager;//6
Entity& player = manager.addEntity();//6
Entity& car = manager.addEntity();//6
Entity& car2 = manager.addEntity();
Entity& wall = manager.addEntity(); //10

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

    map1 = new Map();//5

    player.addComponent<TransformComponent>(WIDTH / 2, 580, 24, 24, 1, 0, 0);
    player.addComponent<SpriteComponent>("imgs/chick_total.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");

    car.addComponent<TransformComponent>(WIDTH + 100, 370, 75, 44, 1, -10, 0);
    car.addComponent<SpriteComponent>("imgs/taxi.png");
    car.addComponent<ColliderComponent>("car");

    car2.addComponent<TransformComponent>(-100, 430, 84, 43, 1, 3, 0);  
    car2.addComponent<SpriteComponent>("imgs/truck.png");
    car2.addComponent<ColliderComponent>("truck");

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

    // if (player.getComponent<TransformComponent>().position.y < HEIGHT / 2) {
    //     player.getComponent<SpriteComponent>().setTex("imgs/chickenright.png");
    // }
    // else if (player.getComponent<TransformComponent>().position.y > HEIGHT / 2){
    //     player.getComponent<SpriteComponent>().setTex("imgs/chickenleft.png");
    // }

    if(Collision::AABB(player.getComponent<ColliderComponent>().collider, car.getComponent<ColliderComponent>().collider)){
        player.getComponent<TransformComponent>().setVelocity(0, 0);       
        cout << "đâm rồi thằng ngu" << endl;
        SDL_Delay(5000);
        isRunning = false;
    }

    if(Collision::AABB(player.getComponent<ColliderComponent>().collider, car2.getComponent<ColliderComponent>().collider)){
        player.getComponent<TransformComponent>().setVelocity(0, 0);
        cout << "đâm rồi thằng ngu" << endl;
        SDL_Delay(5000);
        isRunning = false;
    }
}

void Game::render(){
    SDL_RenderClear(renderer);
    map1->DrawMap(); // ve map trc roi moi den nhan vat//5
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::quit(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "quit game";
}
