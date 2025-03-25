#include "Collision.h"//10
#include "ECS/ColliderComponent.h"//12

using namespace std;

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB){
    // if (recA.x + recA.w >= recB.x && recB.x + recB.w >= recA.x && recA.y + recA.h >= recB.y && recB.y + recB.h >= recA.y){
    //     return true; // phát hiện va chạm
    // }  

    SDL_Rect intersection; // lưu phần giao của hai hình chữ nhật

    if(SDL_IntersectRect(&recA, &recB, &intersection) == SDL_TRUE){ 
        return true;
    }

    return false;
}

bool Collision::AABB(const  ColliderComponent& colA, const ColliderComponent& colB){ 
    if (AABB(colA.collider, colB.collider)) {
        cout << colA.tag << " hit : " << colB.tag << endl; //
        return true;
    }
    else return false;
}