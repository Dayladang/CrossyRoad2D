#include "ScoreSystem.h"
#include "ECS/MiniText.h"
#include <iostream>
#include <sstream>
#include "Game.h"

using namespace std;

ScoreSystem::ScoreSystem() : score(0), highScore(0), lastRow(1000), tileSize(32), startRow(970 / 32) {
    loadHighScore();
}

ScoreSystem::~ScoreSystem() {
    saveHighScore();
}

void ScoreSystem::addScore(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
        saveHighScore();
    }
}

void ScoreSystem::resetScore() {
    score = 0;
}

void ScoreSystem::saveHighScore() {
    fstream file("assets/score/highscore.txt", ios::out);
    if (file.is_open()) {
        file << 0;
        file.close();
    }
}

void ScoreSystem::loadHighScore() {
    fstream file("assets/score/highscore.txt", ios::in);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
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

void LeaderBoard::addPlayerToFile(const string& name, int score){
    v.push_back( {name, score} );

    sort(v.begin(), v.end(), [](const Player a, const Player b) {
        if (a.points == b.points) return a.name > b.name;
        else return a.points > b.points;
    });

    if (v.size() > 10) v.resize(10); // Giới hạn top 10

    //truyền tên và điểm vào file
    fstream file("assets/score/leaderboard.txt", ios::out);
    if (file.is_open()) {
        for (auto& x : v) file << x.name << " " << x.points << endl;
        file.close();
    } 
}

void LeaderBoard::render(){

}