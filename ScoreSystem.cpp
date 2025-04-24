#include "ScoreSystem.h"
#include "ECS/MiniText.h"
#include <iostream>
#include <sstream>
#include "Game.h"

using namespace std;

ScoreSystem::ScoreSystem() : score(0), highScore(0), lastRow(1000), tileSize(32), startRow(970 / 32) {  
    Game::leaderBoard->loadFromFile(); // load file leaderboard.txt
    highScore = Game::leaderBoard->GetTopScore();
}

ScoreSystem::~ScoreSystem() {
    Game::leaderBoard->saveToFile(); // lưu lại file 
}

void ScoreSystem::addScore(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
    }
}

void ScoreSystem::resetScore() {
    score = 0;
}

string ScoreSystem::getScoreText() {
    stringstream ss;
    ss << "Score: " << score << " | High Score: " << highScore;
    return ss.str();
}

int ScoreSystem::getScore() {
    return score;
}

void ScoreSystem::updateScore(int currentPlayerY) {
    
    int currentRow = currentPlayerY / tileSize;// Chuyển đổi vị trí Y thành chỉ số hàng

    //cout << "Current Row: " << currentRow << ", Last Row: " << lastRow << endl;

    if (currentRow == startRow) {
        resetScore();
        lastRow = currentRow;
    }
    
    if (currentRow < lastRow) {     
        int rowsMoved = lastRow - currentRow;// Tính số hàng đã di chuyển
        int points = rowsMoved;
        
        //cout << "Moving up " << rowsMoved << " rows, adding " << points << " points" << endl;
        
        addScore(points);
           
        lastRow = currentRow; // Cập nhật hàng hiện tại
    }
}

void LeaderBoard::addPlayer(const string& name, int score){
    v.push_back( {name, score} );

    sort(v.begin(), v.end(), [](const Player a, const Player b) {
        if (a.scores == b.scores) return a.name > b.name;
        else return a.scores > b.scores;
    });

    if (v.size() > 10) v.resize(10); // Giới hạn top 10
}

void LeaderBoard::loadFromFile() {
    v.clear(); // xóa dữ liệu cũ
    fstream file("assets/score/leaderboard.txt", ios::in);
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            v.push_back( {name, score} );
        }
    }
    file.close();
    cout << "đã lode file \n"; 
    for (auto& x : v) cout << x.name << " " << x.scores << endl;
}

void LeaderBoard::saveToFile() {
    fstream file("assets/score/leaderboard.txt", ios::out);
    if (file.is_open()) {
        for (auto& x : v) file << x.name << " " << x.scores << endl;
    }
    file.close();
    cout << "đã save file \n";
    for (auto& x : v) cout << x.name << " " << x.scores << endl;
}

int LeaderBoard::GetTopScore() {
    return v.empty() ? 0 : v[0].scores;
}

void LeaderBoard::render(){

}