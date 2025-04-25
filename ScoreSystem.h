#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include <SDL2/SDL.h>
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
    // void saveHighScore();
    // void loadHighScore();
    string getScoreText();
    int getScore();

    void updateScore(int currentPlayerY);

};


struct Player {
    string name;
    int scores;
};

class LeaderBoard {

private:
    vector<Player> v;

public :
    
    void addPlayer(const string& name, int score);
    void loadFromFile();
    void saveToFile();
    int GetTopScore();
    vector<Player> getPlayers() {
        return v;
    }

};

#endif