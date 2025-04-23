#ifndef MINITEXT_H
#define MINITEXT_H

#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL_ttf.h"
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "ECS.h"
#include "../AssetManager.h"
#include "../Game.h"
#include <string>

using namespace std;

class MiniText : public Component {

public:
    MiniText(int xpos, int ypos, string text, string font, SDL_Color& color) : labelText(text), font(font), color(color) {
        position.x = xpos;
        position.y = ypos;

        SetLabelText(labelText, font);
    }

    ~MiniText(){

    }

    void SetLabelText(string text, string font){
        SDL_Surface* surface = TTF_RenderText_Solid(Game::assets->GetFont(font), text.c_str(), color);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surface);
        SDL_FreeSurface(surface);

        SDL_QueryTexture(labelTexture, NULL, NULL, &position.w, &position.h);// lấy kích thước của texture
    }

    void draw() override {
        SDL_RenderCopy(Game::renderer, labelTexture, NULL, &position);
    }

    void drawWithBackground(int x, int y, int w, int h, SDL_Texture* bgTexture) { // vẽ text cho màn hình thua
        
        SDL_Rect dstRect = {x, y, w, h};
        position.x = x + (w - position.w) / 2;
        position.y = y + (h - position.h) / 2;
        
        SDL_RenderCopy(Game::renderer, bgTexture, NULL, &dstRect);
    
        SDL_RenderCopy(Game::renderer, labelTexture, NULL, &position);
    }

private:
    SDL_Texture* labelTexture;
    SDL_Rect position;
    SDL_Color color;
    string labelText;
    string font;
};

#endif