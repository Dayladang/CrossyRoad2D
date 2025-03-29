#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "Components.h"
#include "../Game.h"

using namespace std;

class ColliderComponent : public Component {

public:
    SDL_Rect collider; //vị trí và kích thước của collider
    string tag; // gắn tag để phân loại thực thể

    SDL_Texture* tex ; // texture cuar collider
    SDL_Rect srcR, destR; // vị trí và kích thước của texture

    TransformComponent* transform;// đồng bộ hóa vị trí va chạm với vị trí của entityz

    ColliderComponent(string t){
        tag = t;
    }

    ColliderComponent(string t, int xpos, int ypos, int size){
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.w = collider.h = size;
    }

    void init() override {
        if (!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();

        tex = IMG_LoadTexture(Game::renderer, "") ; // Load texture cho collider
        srcR = {0, 0, 32, 32};
        destR = {collider.x, collider.y, collider.w, collider.h};

        //Game::colliders.push_back(this); // thêm collider vào vector các collider
    }

    void update() override {

        if (tag != "terrain"){ // nấu collider không phải là địa hình (terrain) thì cập nhật vị trí của collider theo vị trí của entity
            collider.x = transform->position.x;
            collider.y = transform->position.y;
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }    

        destR.x = collider.x - Game::screen.x;// vị trí của collider cũng phải thay đổi theo vị trí của entity
        destR.y = collider.y - Game::screen.y;
    }

    void draw() override {
        SDL_RenderCopy(Game::renderer, tex, &srcR, &destR);
    }

};

#endif
