#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "C:\Users\ADMIN\OneDrive\Desktop\git_exercise\src\include\SDL2\SDL.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class ScoreSystem {

private:
    int score;
    int highScore;
    int lastRow;
    int startRow;
    int tileSize;

public:
    ScoreSystem();
    ~ScoreSystem();

    void setLastPlayerRow(int row) {
        lastRow = row;
    }

    void addScore(int points);
    void resetScore();
    void saveHighScore();
    void loadHighScore();
    string getScoreText();

    void updateScore(int currentPlayerY);

};


struct Player {
    string name;
    int points;
};

class LeaderBoard {

private:
    string playerName;
public :
    vector<Player> v;

    void addPlayer(const string& name, int score);

    string addPLayerName();

    void render();
};

#endif