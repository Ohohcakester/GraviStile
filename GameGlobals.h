#ifndef GAME_GLOBALS_H_
#define GAME_GLOBALS_H_

#include "gameobjects.h"
#include "Grid.h"

class GameGlobals {
public:
    int refreshCounter;

    int currentStage;

    Player player;
    Keyboard key;
    Door door;
    Camera camera;
    Background background;

    int width;
    int height;

    float zoom;

    bool puzzleComplete;

    Grid grid;

    std::vector<Platform*> platforms;
    std::vector<SpinConnection*> spinConnections;
    std::vector<SwitchConnection*> switchConnections;
    std::vector<Laser*> lasers;
    std::vector<LaserSource*> laserSources;
    std::vector<LaserTarget*> laserTargets;


    GameGlobals();
    ~GameGlobals();

    void onStart();
    void update();
    void refreshMapState();
    void finishRotatingTrigger();
};


#endif