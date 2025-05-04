#ifndef ECS_H //6
#define ECS_H

#include <bits/stdc++.h>

using namespace std;

class Component;
class Entity; 
class Manager;

using ComponentID = size_t ;// dung ten thay the cho kieu du lieu
using Group = size_t; //

inline ComponentID getNewComponentTypeID(){
    static ComponentID lastID = 0;
    return lastID++; // tra ve ID tiep theo 
}


template <typename T> //Template cho phép sử dụng kiểu dữ liệu tùy ý 
inline ComponentID getComponentTypeID() /* noexcept /*vẫn chưa hiểu noexcept là gì*/{
    static ComponentID typeID = getNewComponentTypeID(); // de nhan vao id do ham get phan phoi
    return typeID;
} 


const size_t maxComponents = 32;
const size_t maxGroups = 32;

using ComponentBitSet = bitset<maxComponents>;// quanr lí thông tin dưới dạn 1,0 kiểu đúng hoặc sai ( có vẻ cũng giống bool)
using GroupBitset = bitset<maxGroups>; // 

using ComponentArray = array<Component*, maxComponents>;// cũng giống như mảng một chiều, <kiểu dữ liệ, số lượng phần tử > tên mảng

class Component{

public:
    Entity* entity;

    virtual void init() {}// virtual để khi gọi lớp con ra thì không bị mất dữ liệu
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}//
};

//
class Entity{

private:
    Manager& manager; // them chiếu đến class Manager để quản lí các entity
    bool active = true;
    vector <unique_ptr<Component>> components;// tạo một vector các component

    ComponentArray componentArray = {};//một mảng lưu trữ các components thông qua các một ID kiểu T 
    ComponentBitSet componentBitSet;//một mảng xác định xem có những component nào gắn với entity đang xét
    GroupBitset groupBitset; //quan li nhom cua entity

public:
    Entity(Manager& mManager) : manager(mManager) { //

    }

    void update(){
        for (auto& c : components) c->update();// tror đến hàm update ảo của Component vì c giờ là nắm địa chỉ của vector components kiểu con trỏ quản lí class Component        
    }

    void draw() {
        for (auto& c : components) c->draw();
    }

    bool isActive() { return active; }

    void destroy() { 
        active = false; 
    }

    bool hasGroup(Group mGroup){
        return groupBitset[mGroup]; // trả về true nếu entity có trong Group
    }

    void addGroup(Group mGroup);

    void delGroup(Group mGroup){
        groupBitset[mGroup] = false;
    }

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
        a->entity = this; // trỏ đến entity của class Component và nói rằng đây chính là entity của component mới được tạo ra
        unique_ptr<Component> uPtr { a };
        components.push_back( move( uPtr )); //chuyển quyền sở hữu a từ uPtr vào trong vector các component

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
    vector<Entity*> groupedEntities[maxGroups]; // một mảng 2 chiều lưu trữ các entity theo nhóm ( array< vector<Entity*>, maxGroups> groupedEntities)

public:
    void update(){
        for (auto& e : entities) e->update();
    }
    void draw(){
        for (auto& e : entities) {
            if (e->isActive()) e->draw();
        }
    }

    void refresh(){

        for (int i = 0; i < maxGroups; ++i){
            vector<Entity*>& v = groupedEntities[i];

            auto ngu = remove_if(v.begin(), v.end(), [i](Entity* mEntity){
                return !mEntity->isActive() || !mEntity->hasGroup(i);//trả về true nếu entity không hoạt động hoặc không có trong nhóm
                });

            v.erase(ngu, v.end());
        }

        auto ngu = remove_if( entities.begin(), entities.end(), [](const unique_ptr<Entity> &mEntity) 
            {
                return !mEntity->isActive();// trả về true nếu entity hết hoạt động
            });

        entities.erase(ngu, entities.end() ); // xóa các phần tử được remove_if chuyển xuống dưới
    }

    void addToGroup(Entity* mEntity, Group mGroup){
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    vector<Entity*>& getGroup(Group mGroup){
        return groupedEntities[mGroup]; // trả về vector các entity trong nhóm
    }

    Entity& addEntity(){
        Entity* e = new Entity(*this); // truyền vào manager để quản lí entity
        unique_ptr<Entity> uPtr{ e }; // unique_ptr được tạo ra và quản lý đối tượng Entity mà con trỏ thô e trỏ tới. Quyền sở hữu đối Entity được chuyển từ e sang uPtr 
        entities.push_back(move(uPtr));// move : chuyển quyền sở hữu của uPtr sang entities
        return *e;
    }

};

#endif
