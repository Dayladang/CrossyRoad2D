
#include "Game.cpp"//2
//#include "GameObject.cpp"//4

using namespace std;
/* //thuật toán quicksort
void qsort(int a[],int l, int r){
    if (l >= r) return; // end sort
    int x = a[(l + r) / 2]; //pivot
    int i = l, j = r;
    while(i <= j)
    {
        while (a[i] < x) ++i;
        while (a[j] > x) --j;
        if (i <= j){ // find the numbers in the wrong side and swap them 
            swap(a[i],a[j]); 
            ++i;
            --j;
        }
    }
    qsort(a,l,j);
    qsort(a,i,r);
}
*/

Game* game = NULL;

int main(int argc, char* argv[]){

    const int FPS = 120;//3
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
    return 0;
}