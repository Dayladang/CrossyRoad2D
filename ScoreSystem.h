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
    int lastPlayerY;

public:
    ScoreSystem();
    ~ScoreSystem();

    // int getScore() const {
    //     return score;
    // }

    // int getHighScore() const {
    //     return highScore;
    // }

    // int getLastPlayerY() const {
    //     return lastPLayerY;
    // }

    void setLastPlayerY(int y) {
        lastPlayerY = y;
    }

    void addScore(int points);
    void resetScore();
    void saveHighScore();
    void loadHighScore();
    string getScoreText();

    void updateScore(int currentPlayerY);

};

#endif