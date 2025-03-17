#include "Game.h"
#include "GameObject.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"

using namespace std;

GameObject* player;//4 
GameObject* ngu;//4
Map* map; //5


SDL_Renderer* Game::renderer = NULL; //5

Manager::manager;
auto& newPlayer(manager.addEntity());

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

    player = new GameObject("imgs/car.png", WIDTH, 0);//4
    ngu = new GameObject("imgs/car2.png", WIDTH, 50);//4
    map = new Map();//5

    newPlayer.addComponent<PositionComponent>();
}

void Game::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT) isRunning = false;
    const Uint8* check = SDL_GetKeyboardState(NULL);
    if(check[SDL_SCANCODE_ESCAPE]) isRunning = false;
}

void Game::update(){
    player->Update();//4
    ngu->Update();//4
}

void Game::render(){
    SDL_RenderClear(renderer);
    map->DrawMap(); // ve map trc roi moi den nhan vat//5
    player->Render();//4
    ngu->Render();//4
    SDL_RenderPresent(renderer);
}

void Game::quit(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "quit game";
}
