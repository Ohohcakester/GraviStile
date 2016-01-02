#ifndef STAGE_H_
#define STAGE_H_

#include "Orientations.h"

struct PlatformTemplate {
    int pivotX;
    int pivotY;
    int leftTiles;
    int rightTiles;
    bool rotatable;
    int orientation;
    int spinConnectionIndex;

    PlatformTemplate(int pivotX, int pivotY, int leftTiles, int rightTiles, bool rotatable, int orientation, int spinConnectionIndex = -1) :
        pivotX(pivotX), pivotY(pivotY), leftTiles(leftTiles), rightTiles(rightTiles), rotatable(rotatable), orientation(orientation), spinConnectionIndex(spinConnectionIndex) {
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

struct GameStage {
    int nTilesX;
    int nTilesY;
    std::vector<PlatformTemplate> platforms;
    PlayerTemplate player;
    DoorTemplate door;

    double zoom;

    GameStage(int nTilesX, int nTilesY, std::vector<PlatformTemplate> platforms, PlayerTemplate player, DoorTemplate door, double zoom) : 
        nTilesX(nTilesX), nTilesY(nTilesY), platforms(platforms), player(player), door(door), zoom(zoom) {
    }
};

void initialiseStage(int stage);

#endif
