
#include "Game.cpp"//2

using namespace std;

Game* game = NULL;

int main(int argc, char* argv[]){

    const int FPS = 60;//3
    const int FrameDelay = 1000 / FPS;//3
    Uint32 FrameRun;//3
    int Frametime;//3

    game = new Game();
    game->initSDL(WIDTH, HEIGHT, WINDOW_TITLE);

    while(game->running()){

        FrameRun = SDL_GetTicks64();   //3

        game->handleEvents();
        game->update();
        game->render();

        Frametime = SDL_GetTicks64() - FrameRun;//3

        if(FrameDelay > Frametime) SDL_Delay(FrameDelay - Frametime); //3
         
    }

    game->quit();
    delete game;
    game = NULL;

    return 0;
}