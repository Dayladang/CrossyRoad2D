#include "GameObject.h" //4

using namespace std;

GameObject::GameObject(const char* link, int x, int y){
    objTeture = IMG_LoadTexture(Game::renderer, link);
    if(!objTeture){
        cerr << "loi khoi tao Texture" << SDL_GetError() << endl;
        return;
    }

    xpos = x;
    ypos = y;
}

GameObject::~GameObject(){
    if(objTeture){
        SDL_DestroyTexture(objTeture);
    }
}

void GameObject::Update(){

    xpos--;
    if (xpos <= WIDTH / 2) ypos++;
    //ypos++;

    // srcRect.w = 50;
    // srcRect.h = 50;
    // srcRect.x = 0;
    // srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 75 ;
    destRect.h = 50 ;

    cout << xpos << " " << ypos << endl;//

}

void GameObject::Render(){
    SDL_RenderCopy(Game::renderer, objTeture, NULL, &destRect);
}