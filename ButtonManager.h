#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "ECS/ECS.h"

class ButtonManager {

public:

    static bool playButtonClickedUp;
    static bool playButtonClickedDown;
    static Entity* playButton; //28

    static bool exitGameloseUp;
    static bool exitGameloseDown;

    static bool LeaderBoardButtonUp;
    static bool LeaderBoardButtonDown;
    static bool exitLeaderBoardUp;
    static bool exitLeaderBoardDown;
    static Entity* LeaderBoardButton;

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

    ButtonManager();
    ~ButtonManager();

    static void initButtons();
    static void drawButtons();
    static void resetButtons();

};

#endif