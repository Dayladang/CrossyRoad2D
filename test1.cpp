#include <bits/stdc++.h>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
int main(int argc, char* argv[]){
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;
    SDL_Surface* pic = NULL;
    SDL_Renderer* render = NULL;
    SDL_Texture* texture = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("loi roi" , SDL_GetError());
    }
    else {
        window = SDL_CreateWindow("day la cua so", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 500, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Lỗi khi tạo cửa sổ: %s\n", SDL_GetError());
            SDL_Quit();
            return -1;
        }
            /*
        surface = SDL_GetWindowSurface(window);
        if (surface == NULL) {
            printf("Lỗi khi lấy surface của cửa sổ: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }
        */ //làm việc với renderer và texture thì không cần surface nữa!!!

        //
        SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (render == NULL) {
            printf("Lỗi khi tạo renderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }

        SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
        
        pic = IMG_Load("imgs/mountain.png");    
        if (pic == NULL){
            printf("loi anh roi %s\n", SDL_GetError());
        }
        else {

            SDL_Texture* texture = SDL_CreateTextureFromSurface(render, pic);
            if (texture == NULL) {
                printf("Lỗi khi tạo texture: %s\n", SDL_GetError());
                return -1;
            }
            SDL_FreeSurface(pic);

            // phần này để truy vấn texture xem có lỗi không thôi 
            /*
            Uint32 format;
            int access, w, h;
            SDL_QueryTexture(texture, &format, &access, &w, &h);
            printf("Texture format: %u, access mode: %d, width: %d, height: %d\n", format, access, w, h);
            */
           

            // run 
            SDL_RenderClear(render);
            SDL_RenderCopy(render, texture, NULL, NULL);
            SDL_RenderPresent(render);

            SDL_Delay(5000);

        }

        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(render);           
        SDL_DestroyWindow(window);
        SDL_Quit();
        
    }

    return 0;
}