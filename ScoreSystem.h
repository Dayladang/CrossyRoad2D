#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include <SDL2/SDL.h>
#include <string>
#include <fstream>

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

#endif