#include "ECS.h"

void Entity::addGroup(Group mGroup){  
    groupBitset[mGroup] = true;
    manager.addToGroup(this, mGroup);
}

// xây dựng các hàm của class Manager để quản lí các entities
    void Manager::update(){
        for (auto& e : entities) {
            if (e->isActive()) e->update(); // nếu entity đang hoạt động thì gọi hàm update
        }
    }
    void Manager::draw(){
        for (auto& e : entities) {
            if (e->isActive()) e->draw();
        }
    }

    void Manager::refresh(){

        for (size_t i = 0; i < maxGroups; ++i){
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

    void Manager::addToGroup(Entity* mEntity, Group mGroup){
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    vector<Entity*>& Manager::getGroup(Group mGroup){
        // Xóa bỏ các entity null trước khi trả về
        auto& group = groupedEntities[mGroup];
        auto it = std::remove_if(group.begin(), group.end(), [](Entity* e) { 
            return e == nullptr; 
        });
            
        group.erase(it, group.end());
        return group; // trả về vector các entity trong nhóm
    }

    Entity& Manager::addEntity(){
        Entity* e = new Entity(*this); // truyền vào manager để quản lí entity
        unique_ptr<Entity> uPtr{ e }; // unique_ptr được tạo ra và quản lý đối tượng Entity mà con trỏ thô e trỏ tới. Quyền sở hữu đối Entity được chuyển từ e sang uPtr 
        entities.push_back(move(uPtr));// move : chuyển quyền sở hữu của uPtr sang entities
        return *e;
    }