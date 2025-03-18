#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "Components.h"

struct Vector2D {
    int x, y;
    Vector2D() : x(0), y(0) {}
    Vector2D(int x, int y) : x(x), y(y) {}

    Vector2D& operator+=(const Vector2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
};

class PositionComponent : public Component {

    private:
        Vector2D position;
        Vector2D velocity;
    
    public:

        PositionComponent(){
            position.x = 0;
            position.y = 0;
            velocity.x = 0;
            velocity.y = 0;
        }

        PositionComponent(int x, int y, int vx, int vy){
            position.x = x;
            position.y = y;
            velocity.x = vx;
            velocity.y = vy;
        }

        int x() { return position.x; }
        void x(int x) { position.x = x; }
        int y() { return position.y; }
        void y(int y) { position.y = y; }
    
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