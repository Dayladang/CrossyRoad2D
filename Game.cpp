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
Entity& character = manager.addEntity();//6
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

    player.addComponent<TransformComponent>();
    player.addComponent<SpriteComponent>("imgs/car.png");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");

    character.addComponent<TransformComponent>(WIDTH / 2, 0, -1, 0);
    character.addComponent<SpriteComponent>("imgs/car2.png");
    character.addComponent<ColliderComponent>("character");

    wall.addComponent<TransformComponent>(300, 300, 300, 20, 1);
    wall.addComponent<SpriteComponent>("imgs/dirt.png");
    wall.addComponent<ColliderComponent>("wall");

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

    if (player.getComponent<TransformComponent>().position.y < HEIGHT / 2) {
        player.getComponent<SpriteComponent>().setTex("imgs/car.png");
    }
    else if (player.getComponent<TransformComponent>().position.y > HEIGHT / 2){
        player.getComponent<SpriteComponent>().setTex("imgs/car2.png");
    }

    if(Collision::AABB(player.getComponent<ColliderComponent>().collider, wall.getComponent<ColliderComponent>().collider)){
        player.getComponent<TransformComponent>().setVelocity(0, 0);
        cout << "đâm rồi thằng ngu" << endl;
        SDL_Delay(1000);
        isRunning  = false;
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
