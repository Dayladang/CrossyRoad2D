#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <SDL2/SDL.h>

class TileComponent : public Component {

public :
    TransformComponent* transform;
    SpriteComponent* sprite;

    SDL_Rect tileRect;
    int tileID; //
    const char* path;

    TileComponent() = default;

    TileComponent(int x, int y, int w, int h, int id){
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        if (tileID == 0){
            path = "imgs/water.png";
        }
        else if (tileID == 1){
            path = "imgs/dirt.png";
        }
        else if (tileID == 2){
            path = "imgs/grass.png";
        }
    }

    void init() override {
        entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1, 0, 0);
        transform = &entity->getComponent<TransformComponent>();


        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};

#endif