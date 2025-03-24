#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

using namespace std;

class Vector2D{

public:
    float x;
    float y;

    Vector2D();
    Vector2D(int x, int y);

    Vector2D& Add(const Vector2D& vec); // 
    Vector2D& Subtract(const Vector2D& vec); //-

    friend Vector2D& operator+ (Vector2D& v1, Vector2D& v2); // friend là kiểu hàm có thể truy cập được các biến pirvate của class; và không là hàm thành viên ; có thể có định nghĩa trong và ngoài lớp => khi dùng không ần tenlop::tenham
    friend Vector2D& operator- (Vector2D& v1, Vector2D& v2); // các phép tính với 2 vector. Mấy cái này sẽ trả được 1 trong 3 hàm trên định nghĩa hộ

    Vector2D& operator+= (const Vector2D& vec);
    Vector2D& operator-= (const Vector2D& vec);

    Vector2D& operator* (const int& i); nhân vector với một số nguyên
    Vector2D& Zero(); // gán vector về 0

    friend ostream& operator<< (ostream& stream, const Vector2D& vec); // in ra Vector
};

#endif