#include "ScoreSystem.h"

using namespace std;

ScoreSystem::ScoreSystem() : score(0), highScore(0), lastPlayerY(970){
    loadHighScore();
}

ScoreSystem::~ScoreSystem() {
    saveHighScore();
}

void ScoreSystem::addScore(int points){
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
    cout << "Before update: currentPlayerY=" << currentPlayerY 
         << ", lastPlayerY=" << lastPlayerY << endl;    

    if (currentPlayerY < lastPlayerY && lastPlayerY > 0) {
        int points = (int) round( (lastPlayerY - currentPlayerY) / 5 ); //tính điểm dựa trên khoảng cách giữa vị trí cũ và vị trí hiện tại
        addScore(points);
    }

    lastPlayerY = currentPlayerY; //cập nhật vị trí cũ

    cout << "After update: lastPlayerY=" << lastPlayerY 
         << ", score=" << score << endl;

}