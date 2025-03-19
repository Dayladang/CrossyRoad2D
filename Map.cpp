#include "Map.h" //5

int maplv1[2][1] = {
    {0},
    {1}
};

Map::Map(){
    dirt = IMG_LoadTexture(Game::renderer, "imgs/dirt.png");
    grass= IMG_LoadTexture(Game::renderer, "imgs/grass.png");
    water = IMG_LoadTexture(Game::renderer, "imgs/water.png");

    LoadMap(maplv1);

    src.x = src.y = 0;
    src.w = dest.w = WIDTH;
    src.h = dest.h = HEIGHT / 2;

    dest.x = dest.y = 0;
}

void Map::LoadMap(int arr[2][1]){
    for (int i = 0; i < 2; ++i){
        for (int j = 0; j  < 1; ++j){
            map[i][j] = arr[i][j];    // truyen gia tri vao map
        }
    }
}

void Map::DrawMap(){
    int type = 0;

    for (int i = 0; i < 2; ++i){
        for (int j = 0; j  < 1; ++j){
            
            type = map[i][j];

            dest.x = j * WIDTH;
            dest.y = i * (HEIGHT / 2);

            if (type == 0) SDL_RenderCopy(Game::renderer, dirt, &src, &dest);
            else if (type == 1) SDL_RenderCopy(Game::renderer, grass, &src, &dest);
            else if (type == 2) SDL_RenderCopy(Game::renderer, water, &src, &dest);

        }
    }
}