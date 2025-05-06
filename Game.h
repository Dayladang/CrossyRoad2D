#ifndef GRAPHIC_H // ten cua thu vien //2
#define GRAPHIC_H

#include <iostream>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "AssetManager.h"
#include "ScoreSystem.h"

using namespace std;

constexpr int WIDTH = 512; // constexpr giá trị của WIDTH được xác định ngay trong lúc viên dịch -> giúp chương trình chạy nhanh và an toàn hơn
constexpr int HEIGHT = 512;
extern const char WINDOW_TITLE[]; // extern là khai báo biến toàn cục, sẽ được định nghĩa trong file khác và khai báo ở đây để sử dụng trong file này tránh việc tạo ra bản sao mới "multiple definition"

class AssetManager;
class ColliderComponent;

class Game{

public:
    Game();
    ~Game();

    void initSDL(const int WIDTH, const int HEIGHT, const char* WINDOW_TITLE);
    void handleEvents();
    void update();
    bool running(){ return isRunning;};
    void render();
    static void resetGame();
    void quit();

    static SDL_Renderer* renderer; // renderer tổng, sẽ tồn tại đến khi chương trình két thúc //5 luon
    static SDL_Event event;

    static bool isRunning;

    static bool isSquashed;

    static SDL_Rect screen;
    static AssetManager* assets;

    static bool playButtonClickedUp;
    static bool playButtonClickedDown;
    static Entity* playButton; //28

    static bool isLogoActive;

    static bool UIwriteName;
    static bool exitGameloseUp;
    static bool exitGameloseDown;
    static string playerName;
    static bool isTypingName;

    static LeaderBoard* leaderBoard;
    static bool LeaderBoardButtonUp;
    static bool LeaderBoardButtonDown;
    static bool exitLeaderBoardUp;
    static bool exitLeaderBoardDown;
    static Entity* LeaderBoardButton;

    static ScoreSystem* scoreSystem; 
    static int currentScore;

    static bool isPausedUp;
    static bool isPausedDown;
    static Entity* PauseButton;
    static Entity* PauseScreen;

    static Entity* quitGame;
    static bool quitGameUp;
    static bool quitGameNoUp;
    static bool quitGameNoDown;
    static bool quitGameYesUp;
    static bool quitGameYesDown;

    static Entity* mutedButton;
    static bool unMutedButtonUp;
    static bool unMutedButtonDown;
    static bool MutedButtonUp;
    static bool MutedButtonDown;

    enum groupLabels : size_t { // size_t được định nghĩa trong ECS.h là Group
        groupMap,
        groupPlayer,
        groupColliders,
        groupVehicles,
        groupDangers
    };

private:   
    SDL_Window* window;  
};

#endif// GRAPHIC_H