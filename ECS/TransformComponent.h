#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component {
    
    public:

        Vector2D position;
        Vector2D velocity;

        TransformComponent(){
            position.x = 0;
            position.y = 0;
            velocity.x = 0;
            velocity.y = 0;
        }

        TransformComponent(float x, float y, int vx, int vy){
            position.x = x;
            position.y = y;
            velocity.x = vx;
            velocity.y = vy;
        }
    
        void update() override { // ghi đè lên , định nghĩa lại hàm init() từ lớp Component cha
            //xpos++;
            //ypos++;
            position += velocity;
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