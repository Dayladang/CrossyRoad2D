#include "Vector2D.h"

using namespace std;

Vector2D::Vector2D(){
    x = 0.0;
    y = 0.0;
}

Vector2D::Vector2D(float x1, float y1){
    x = x1;
    y = y1;
}

//
Vector2D& Vector2D::Add(const Vector2D& vec1){
    x += vec1.x;
    y += vec1.y;

    return this*; // tra về vector đã được cộng với vec1
}

Vector2D& Vector2D::Subtract(const Vector2D& vec1){
    x -= vec1.x;
    y -= vec1.y;

    return this*; // tra về vector đã được cộng với vec1
}

//
Vector2D& operator+ (Vector2D& v1, Vector2D& v2){
    return v1.Add(v2);
}

Vector2D& operator- (Vector2D& v1, Vector2D& v2){
    return v1.Subtract(v2);
}

//
Vector2D& Vector2D::operator+= (const Vector2D& vec){
    return Add(vec); // có thể thêm this-> ở trước để cho rõ ràng
}

Vector2D& Vector2D::operator-= (const Vector2D& vec){
    return Subtract(vec);
}

//
ostream& operator<< (ostream& stream, const Vector2D& vec){
    stream << "ngu the (" << vec.x << "," << vec.y << " )";
    return stream;
}


