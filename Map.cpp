#include "Map.h" //5
#include "Game.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "ECS/ECS.h" 
#include "ECS/Components.h"

using namespace std;

extern Manager manager; // sử dụng tiếp manager đã được khai báo ở Game.cpp

Map::Map(const char* maplink, int mapscale, int tilesize){
    mapLink = maplink;
    mapScale = mapscale;
    tileSize = tilesize;
}

Map::~Map(){

}

// void Map::LoadMap(string path, int sizeX, int sizeY){
//     char c; // lưu giá trị của tile
//     fstream mapFile;
//     mapFile.open(path, fstream::in);

//     int srcX, srcY;

//     for (int y = 0; y < sizeY; ++y){
//         for (int x = 0; x < sizeX; ++x){
//             mapFile.get(c); // lấy giá trị của tile
//             srcY = atoi(&c) * 21; // chuyển giá trị của tile thành số nguyên và nhân với 32
//             mapFile.get(c);
//             srcX = atoi(&c) * 21;
//             Game::AddTile(srcX, srcY, x * 21, y * 21); // chuyển giá trị của tile thành số nguyên và thêm vào map
//             mapFile.ignore();// bỏ qua kí tự xuống dòng
//         }
//     }
// }

void Map::LoadMap(std::string path, int sizeX, int sizeY, int gridWidth) {
    ifstream mapFile(path);
    if (!mapFile.is_open()) {
        std::cerr << "Failed to open map file: " << path << std::endl;
        return;
    }

    string line;
    vector<vector<int>> mapData;

    // Đọc dữ liệu từ file và lưu vào vector 2D
    while (getline(mapFile, line)) {
        istringstream ss(line); // 
        vector<int> row;
        string value;

        while (getline(ss, value, ',')) {
            row.push_back(stoi(value)); // Chuyển giá trị từ chuỗi sang số nguyên
        }

        mapData.push_back(row);
    }

    mapFile.close();

    // Duyệt qua dữ liệu bản đồ và thêm các tile
    for (int y = 0; y < sizeY; ++y) {
        for (int x = 0; x < sizeX / 2; ++x) {
            int tileCode = mapData[y][x];
            int srcX = tileCode % gridWidth; // Cột trong sprite sheet
            int srcY = tileCode / gridWidth; // Hàng trong sprite sheet

            AddTile(srcX * tileSize, srcY * tileSize, x * (tileSize * mapScale), y * (tileSize * mapScale)); // Thêm tile vào bản đồ
            
        }
    }

    // Duyệt qua bản đồ thêm các collider
    for (int y = sizeY; y < sizeY * 2; ++y) {
        for (int x = 0; x < sizeX / 2; ++x) {
            int tileCode = mapData[y][x];
            if (tileCode == 1){
                Entity& block = manager.addEntity();
                block.addComponent<ColliderComponent>("terrain", x * (tileSize * mapScale), (y - sizeY) * (tileSize * mapScale), tileSize * mapScale);
                block.addGroup(Game::groupColliders);

                // cout << "Collider entity added at position: (" 
                // << x * (tileSize * mapScale)<< ", " 
                // << (y - sizeY) * (tileSize * mapScale) << ")" << std::endl;
            }
        }
    }
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos){ //18
    Entity& Map = manager.addEntity(); //13
    Map.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, mapLink);
    Map.addGroup(Game::groupMap);
}
