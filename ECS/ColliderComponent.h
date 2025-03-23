#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <string>
#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include "Components.h"

using namespace std;

class ColliderComponent : public Component {

public:
    SDL_Rect collider; //vị trí và kích thước của collider
    string tag; // gắn tag để phân loại thực thể

    TransformComponent* transform;// đồng bộ hóa vị trí va chạm với vị trí của entityz

    ColliderComponent(string t){
        tag = t;
    }

    void init() override {
        if (!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        collider.x = transform->position.x;
        collider.y = transform->position.y;
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale; 

        // In thông tin collider để kiểm tra
        cout << "Collider: (" << collider.x << ", " << collider.y << ", " 
                    << collider.w << ", " << collider.h << ")" << endl;
    }

};

#endif
