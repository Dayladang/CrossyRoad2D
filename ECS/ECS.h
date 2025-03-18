#ifndef ECS_H //6
#define ECS_H

#include <bits/stdc++.h>

using namespace std;

class Component;
class Entity;

using ComponentID = size_t ;// dung ten tah ythe cho kieu du lieu

inline ComponentID getComponentTypeID(){
    static ComponentID lastID = 0;
    return lastID++; // tra ve ID tiep theo 
}


template <typename T> //Template cho phép sử dụng kiểu dữ liệu tùy ý 
inline ComponentID getComponentTypeID() noexcept /*vẫn chưa hiểu noexcept là gì*/{
    //static_assert (is_base_of <Component, T>::value, "");//
    static ComponentID typeID = getComponentTypeID(); // de nhan vao id do ham get phan phoi
    return typeID;
} 


const size_t maxComponents = 32;

using ComponentBitSet = bitset<maxComponents>;// quanr lí thông tin dưới dạn 1,0
using ComponentArray = array<Component*, maxComponents>;// cũng giống như mảng một chiều, <kiểu dữ liệ, số lượng phần tử > tên mảng

class Component{

public:
    Entity* entity;

    virtual void init() {}// virtual để khi gọi lớp con ra thì không bị mất dữ liệu
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}//
};

class Entity{

private:
    bool active = true;
    vector <unique_ptr<Component>> components;// tạo một vector các component

    ComponentArray componentArray = {};//một mảng lưu trữ các components thông qua các một ID kiểu T 
    ComponentBitSet componentBitSet;//một mảng xác định xem có những component nào gắn với entity đang xét
public:
    void update(){
        for (auto& c : components) c->update();// tror đến hàm update ảo của Component vì c giờ là nắm địa chỉ của vector components kiểu con trỏ quản lí class Component        
    }
    void draw() {
        for (auto& c : components) c->draw();
    }
    bool isActive() { return active; }
    void destroy() { active = false; }

    //kiểm tra xem entity có một component cụ thể nào không
    template <class T> 
    bool hasComponent() const {
        return componentBitSet[getComponentTypeID<T>()];// `componentBitSet` là một mảng hoặc một tập hợp nào đó lưu trữ thông tin về các component.
        // `getComponentTypeID<T>` là một hàm template khác, trả về ID hoặc chỉ số tương ứng với kiểu T.
    }

    // thêm component cho entity
    template <typename T, typename... TArgs>//'typename... Targs ' dấu '...' nghĩa là nó có thể nhận một danh sách các kiểu dữ liệu (không kiểu nào hoặc nhiều kiểu)
    T& addComponent(TArgs&&... mArgs){//Hàm trả về một tham chiếu đến đối tượng kiểu T
        T* a = new T(forward<TArgs>(mArgs)...);//Nếu mArgs là r-value, nó sẽ được truyền như r-value bằng std::forward -> tăng hiệu suất. Tạm thời hiểu vậy
        a->entity = this; //
        unique_ptr<Component> uPtr { a };
        components.emplace_back( move( uPtr ));

        componentArray[getComponentTypeID<T>()] = a;//gán ID theo kiểu T làm chỉ số lưu trữ ID của component. c là con trỏ component lấy dc từ hàm add và sẽ dc lưu trong componentArray
        componentBitSet[getComponentTypeID<T>()] = true; // true là component đã dc thêm vào hệ thống

        a->init();// khởi tạo component mới dc thêm vào
        return *a;// trả về component dc thêm và CHO PHÉP THAO TÁC  với nó
    }

    template<typename T> 
    T& getComponent() const {
        auto ptr = componentArray[getComponentTypeID<T>()];
        return *static_cast<T*>(ptr);// trả về chuyển kiểu dữ liệu kiểu của component từ Component* sang T*
    } 

};

// xây dựng class để quản lí các entities
class Manager{

private:
    vector< unique_ptr<Entity> > entities; // tạo một vector các entity

public:
    void update(){
        for (auto& e : entities) e->update();
    }
    void draw(){
        for (auto& e : entities) e->draw();
    }

    void refresh(){
        auto ngu = remove_if( entities.begin(), entities.end(), [](const unique_ptr<Entity> &mEntity) 
            {
                return !(mEntity->isActive());// trả về true nếu entity hết hoạt động
            });

        entities.erase(ngu, entities.end() ); // xóa các phần tử được remove_if chuyển xuống dưới
    }

    Entity& addEntity(){
        Entity* e = new Entity();
        unique_ptr<Entity> uPtr{ e }; // unique_ptr được tạo ra và quản lý đối tượng Entity mà con trỏ thô e trỏ tới. Quyền sở hữu đối Entity được chuyển từ e sang uPtr 
        entities.emplace_back(move(uPtr));// move : chuyển quyền sở hữu của uPtr sang entities
        return *e;
    }
};

#endif
