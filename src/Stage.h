#ifndef STAGE_H_
#define STAGE_H_

#include <vector>
#include "Orientations.h"

struct PlatformTemplate {
    int pivotX;
    int pivotY;
    int leftTiles;
    int rightTiles;
    bool rotatable;
    int orientation;
    int spinConnectionIndex;
    int id;
    int pivotSwitchConnectionIndex;
    int platformSwitchConnectionIndex;
    bool isDisabled;
    bool isRotationDisabled;

    PlatformTemplate(int pivotX, int pivotY, int leftTiles, int rightTiles, bool rotatable, int orientation,
        int spinConnectionIndex = -1, int id = -1, int pivotSwitchConnectionIndex = -1, int platformSwitchConnectionIndex = -1,
        bool isDisabled = false, bool isRotationDisabled = false) :
        pivotX(pivotX), pivotY(pivotY), leftTiles(leftTiles), rightTiles(rightTiles), rotatable(rotatable), orientation(orientation),
        spinConnectionIndex(spinConnectionIndex), id(id), pivotSwitchConnectionIndex(pivotSwitchConnectionIndex), 
        platformSwitchConnectionIndex(platformSwitchConnectionIndex), isDisabled(isDisabled), isRotationDisabled(isRotationDisabled) {
    }

    PlatformTemplate &_spinConnectionIndex(int v) { spinConnectionIndex = v; return *this; }
    PlatformTemplate &_id(int v) { id = v; return *this; }
    PlatformTemplate &_pivotSwitchConnectionIndex(int v) { pivotSwitchConnectionIndex = v; return *this; }
    PlatformTemplate &_platformSwitchConnectionIndex(int v) { platformSwitchConnectionIndex = v; return *this; }
    PlatformTemplate &_isDisabled(bool v) { isDisabled = v; return *this; }
    PlatformTemplate &_isRotationDisabled(bool v) { isRotationDisabled = v; return *this; }

    static PlatformTemplate create(int pivotX, int pivotY, int leftTiles, int rightTiles, bool rotatable, int orientation) {
        return PlatformTemplate(pivotX, pivotY, leftTiles, rightTiles, rotatable, orientation);
    }
};

struct DoorTemplate {
    int x;
    int y;
    int orientation;

    DoorTemplate(int x, int y, int orientation) : x(x), y(y), orientation(orientation) {}
};

struct PlayerTemplate {
    int x;
    int y;
    int orientation;

    PlayerTemplate(int x, int y, int orientation) : x(x), y(y), orientation(orientation) {}
};

struct LaserSourceTemplate {
    int position;
    int orientation;
    int platformId;

    LaserSourceTemplate(int position, int orientation, int platformId) :
        position(position), orientation(orientation), platformId(platformId) {}
};

struct LaserTargetTemplate {
    int position;
    int orientation;
    int platformId;
    int switchConnectionIndex;

    LaserTargetTemplate(int position, int orientation, int platformId, int switchConnectionIndex) :
        position(position), orientation(orientation), platformId(platformId), switchConnectionIndex(switchConnectionIndex) {}
};

struct GameStage {
    std::vector<PlatformTemplate> platforms;
    std::vector<LaserSourceTemplate> laserSources;
    std::vector<LaserTargetTemplate> laserTargets;
    PlayerTemplate player;
    DoorTemplate door;
    std::string name;

    double zoom;

    GameStage(std::string name, std::vector<PlatformTemplate> platforms, PlayerTemplate player, DoorTemplate door, double zoom,
        std::vector<LaserSourceTemplate> laserSources = {}, std::vector<LaserTargetTemplate> laserTargets = {}) :
        name(name), platforms(platforms), player(player), door(door), zoom(zoom), laserSources(laserSources), laserTargets(laserTargets) {
    }
};

void initialiseFromStageObject(GameStage gameStage);
void initialiseStage(int stage);

#endif
