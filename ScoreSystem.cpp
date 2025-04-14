#include "ScoreSystem.h"
#include <iostream>
#include <sstream>

using namespace std;

ScoreSystem::ScoreSystem() : score(0), highScore(0), lastRow(1000), tileSize(32), startRow(0) {
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
        file << highScore;
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

void ScoreSystem::updateScore(int currentPlayerY) {
    
    int currentRow = currentPlayerY / tileSize;// Chuyển đổi vị trí Y thành chỉ số hàng

    //cout << "Current Row: " << currentRow << ", Last Row: " << lastRow << endl;

    if (currentRow == startRow) {
        cout << "PLayer is on the starting row \n";
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