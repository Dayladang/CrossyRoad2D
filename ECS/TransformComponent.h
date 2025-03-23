#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component {
    
    public:

        Vector2D position;
        Vector2D velocity;

        int height = 16;
        int width = 32;
        int scale = 2;

        int speed = 1;

        TransformComponent(){
            position.x = 0;
            position.y = 0;
            
        }

        TransformComponent(int s){
            position.x = 0;
            position.y = 0;
            scale = s;
        }

        TransformComponent(int x, int y, int vx, int vy){
            position.x = x;
            position.y = y;
            velocity.x = vx;
            velocity.y = vy;
        }

        TransformComponent(int x, int y, int h, int w, int s){
            position.x = x;
            position.y = y;
            height = h;
            width = w;
            scale = s;
        }
    
        void init() override {
            // velocity.x = 0;
            // velocity.y = 0; // nếu muốn vật có velocity ngay từ đầu thì không dùng dòng này vid sẽ bị đè lên = 0 nếu khởi động chương trình 
        }

        void update() override { // ghi đè lên , định nghĩa lại hàm init() từ lớp Component cha
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
        
        void setPosition(int x, int y) {
            position.x = x;
            position.y = y;
        }

        void setVelocity(int vx, int vy) {
            velocity.x = vx;
            velocity.y = vy;
        }

        Vector2D getPosition() const { return position; }

};
    

#endif