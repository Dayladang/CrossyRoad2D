#include "Game.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"

using namespace std;

Map* map1; //5

SDL_Renderer* Game::renderer = NULL; //5

Manager manager;//6
Entity& player = manager.addEntity();//6
Entity& character = manager.addEntity();//6

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

    player.addComponent<TransformComponent>(WIDTH + 100, 0, -1, 0);
    player.addComponent<SpriteComponent>("imgs/car.png");
    character.addComponent<TransformComponent>(WIDTH / 2, HEIGHT, 0, 0);
    character.addComponent<SpriteComponent>("imgs/car2.png");
}

void Game::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);

    if(event.type == SDL_QUIT) isRunning = false;

    else if ( event.type == SDL_KEYDOWN ){
        const Uint8* check = SDL_GetKeyboardState(NULL);
        if(check[SDL_SCANCODE_ESCAPE]) isRunning = false;
        else if (check[SDL_SCANCODE_UP]) character.getComponent<TransformComponent>().setVelocity(0, -1);
        else if (check[SDL_SCANCODE_DOWN]) character.getComponent<TransformComponent>().setVelocity(0, 1);
        else if (check[SDL_SCANCODE_RIGHT]) character.getComponent<TransformComponent>().setVelocity(1, 0);
        else if (check[SDL_SCANCODE_LEFT]) character.getComponent<TransformComponent>().setVelocity(-1, 0);
    }

    else if ( event.type == SDL_KEYUP ){
        character.getComponent<TransformComponent>().setVelocity(0, 0); // để vật không bị trôi 
    }
}

void Game::update(){
    manager.refresh();//8
    manager.update();//6 

    if (character.getComponent<TransformComponent>().position.y < HEIGHT / 2) {
        character.getComponent<SpriteComponent>().setTex("imgs/car.png");
    }
    else if (character.getComponent<TransformComponent>().position.y > HEIGHT / 2){
        character.getComponent<SpriteComponent>().setTex("imgs/car2.png");
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
