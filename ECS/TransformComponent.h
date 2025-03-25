#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component {
    
    public:

        Vector2D position;
        Vector2D velocity;

        int height ;
        int width ;
        int scale ;

        int speed = 1;

        TransformComponent(){
            position.Zero();
            
        }

        TransformComponent(int s){
            position.Zero();
            scale = s;
        }

        TransformComponent(int x, int y, int vx, int vy){
            position.x = x;
            position.y = y;
            velocity.x = vx;
            velocity.y = vy;
        }

        TransformComponent(int x, int y, int w, int h, int s, int vx, int vy){
            position.x = x;
            position.y = y;
            width = w;
            height = h;
            scale = s;
            velocity.x = vx;
            velocity.y = vy;
        }
    
        void init() override {
            //velocity.Zero(); // nếu muốn vật có velocity ngay từ đầu thì không dùng dòng này vid sẽ bị đè lên = 0 nếu khởi động chương trình 
        }

        void update() override { // ghi đè lên , định nghĩa lại hàm init() từ lớp Component cha
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }

        Vector2D getPosition() const { return position; }

};
    

#endif