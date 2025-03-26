#include "Map.h" //5
#include "Game.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

Map::Map(){

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
        for (int x = 0; x < sizeX; ++x) {
            int tileCode = mapData[y][x];
            int srcX = tileCode % gridWidth; // Cột trong sprite sheet
            int srcY = tileCode / gridWidth; // Hàng trong sprite sheet

            Game::AddTile(srcX * 32, srcY * 32, x * 32, y * 32); // Thêm tile vào bản đồ
            
        }
    }
}
