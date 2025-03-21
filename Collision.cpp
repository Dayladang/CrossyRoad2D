#include "Collision.h"//10

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB){
    if (recA.x + recA.w >= recB.x && recB.x + recB.w >= recA.x && recA.y + recA.h >= recB.y && recB.y + recB.h >= recA.y){
        return true; // phát hiện va chạm
    }  

    return false;
}