#ifndef GAME_GLOBALS_H_
#define GAME_GLOBALS_H_

#include "gameobjects.h"
#include "Grid.h"

class GameGlobals {
public:
    std::string stageName;

    int refreshCounter;

    int currentStage;

    Player player;
    Door door;
    Background background;
    AbstractCamera* camera;

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
    std::vector<sfx::Sfx*> sfxList;

    GameGlobals();
    ~GameGlobals();

    void assignNewCamera(AbstractCamera* camera);

    void onStart();
    void update();
    void updateAllSfx();
    void refreshMapState();
    void finishRotatingTrigger();

    void spawnNewSfx(sfx::Sfx* newSfx);

private:
    int sfxArrayClearCounter;
};


#endif