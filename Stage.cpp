#include <iostream>
#include <vector>
#include <map>
#include "globals.h"
#include "Stage.h"
#include "SpinConnection.h"
#include "GameGlobals.h"
#include "gameobjects.h"

void linkPlatforms(GameStage* gameStage, std::vector<Platform*>* _platforms, std::vector<SpinConnection*>* spinConnections) {
    std::vector<Platform*>& platforms = *_platforms;

    std::map<int, std::vector<Platform*>> platformGroups;
    int nPlatforms = gameStage->platforms.size();
    for (int i = 0; i < nPlatforms; ++i) {
        int index = gameStage->platforms[i].spinConnectionIndex;
        if (index == -1) continue;
        auto vec = platformGroups.find(index);
        if (vec == platformGroups.end()) {
            platformGroups.emplace(index, std::vector<Platform*> {platforms[i]});
        }
        else {
            vec->second.push_back(platforms[i]);
        }
    }

    // link.
    for (auto itr = platformGroups.begin(); itr != platformGroups.end(); ++itr) {
        spinConnections->push_back(createSpinConnection(itr->second));
    }
}

void setupLasers(GameStage* gameStage, std::map<int, Platform*>* _platformsById) {
    std::map<int, Platform*>& platformsById = *_platformsById;

    for (size_t i = 0, n = gameStage->laserSources.size(); i < n; ++i) {
        LaserSourceTemplate* laserSourceTemplate = &gameStage->laserSources[i];
        Platform* platform = platformsById[laserSourceTemplate->platformId];
        LaserSource* laserSource = new LaserSource(platform, laserSourceTemplate->orientation, laserSourceTemplate->position);
        Laser* laser = new Laser(laserSource);

        laserSource->setupLaser(laser);
        platform->addLaserSource(laserSource);

        game.laserSources.push_back(laserSource);
        game.lasers.push_back(laser);
    }

    std::map<int, std::vector<Platform*>> switchPivotGroups;
    std::map<int, std::vector<Platform*>> switchPlatformGroups;
    for (size_t i = 0, n = gameStage->platforms.size(); i < n; ++i) {
        Platform* platform = game.platforms[i];
        int index;

        // organise into switch pivot groups
        index = gameStage->platforms[i].pivotSwitchConnectionIndex;
        if (index != -1) {
            auto vec = switchPivotGroups.find(index);
            if (vec == switchPivotGroups.end()) {
                switchPivotGroups.emplace(index, std::vector < Platform* > {platform});
            }
            else {
                vec->second.push_back(platform);
            }
        }

        // organise into switch platform groups
        index = gameStage->platforms[i].platformSwitchConnectionIndex;
        if (index != -1) {
            auto vec = switchPlatformGroups.find(index);
            if (vec == switchPlatformGroups.end()) {
                switchPlatformGroups.emplace(index, std::vector < Platform* > {platform});
            }
            else {
                vec->second.push_back(platform);
            }
        }
    }


    for (size_t i = 0, n = gameStage->laserTargets.size(); i < n; ++i) {
        LaserTargetTemplate* laserTargetTemplate = &gameStage->laserTargets[i];
        Platform* platform = platformsById[laserTargetTemplate->platformId];
        LaserTarget* laserTarget = new LaserTarget(platform, laserTargetTemplate->orientation, laserTargetTemplate->position);
        platform->addLaserTarget(laserTarget);
        game.laserTargets.push_back(laserTarget);

        // Create Switch Connection.
        int index = laserTargetTemplate->switchConnectionIndex;
        std::vector<Platform*> switchPlatforms;
        std::vector<Platform*> switchPivots;
        {
            auto vec = switchPlatformGroups.find(index);
            if (vec != switchPlatformGroups.end()) {
                switchPlatforms.swap(vec->second);
            }
        }
        {
            auto vec = switchPivotGroups.find(index);
            if (vec != switchPivotGroups.end()) {
                switchPivots.swap(vec->second);
            }
        }

        game.switchConnections.push_back(createSwitchConnection(laserTarget, switchPlatforms, switchPivots));
    }
}

void initialiseGrid(std::vector<Platform*>* platforms) {

    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();

    for (size_t i = 0, n = platforms->size(); i < n; ++i) {
        Platform* platform = (*platforms)[i];
        int length = std::max(platform->leftTiles, platform->rightTiles) + 1;
        int x1 = platform->cx - length;
        int x2 = platform->cx + length;
        int y1 = platform->cy - length;
        int y2 = platform->cy + length;
        if (x1 < minX) minX = x1;
        if (y1 < minY) minY = y1;
        if (x2 > maxX) maxX = x2;
        if (y2 > maxY) maxY = y2;
    }

    game.grid = Grid(minX, minY, maxX, maxY);
}

void initialiseFromStageObject(GameStage gameStage) {
    int nPlatforms = gameStage.platforms.size();

    std::map<int, Platform*> platformsById;
    std::vector<Platform*> platforms(nPlatforms);
    for (int i = 0; i < nPlatforms; ++i) {
        PlatformTemplate* t = &gameStage.platforms[i];
        Platform* platform = new Platform(t->pivotX, t->pivotY, t->leftTiles, t->rightTiles, t->rotatable, t->orientation, t->isDisabled, t->isRotationDisabled);
        platforms[i] = platform;

        if (t->id != -1) {
            platformsById.emplace(t->id, platform);
        }
    }
    game.platforms.swap(platforms);

    initialiseGrid(&game.platforms);
    game.player = Player(gameStage.player.x, gameStage.player.y);
    game.player.setOrientation(game.player.orientation);
    game.zoom = (float)(gameStage.zoom);
    Door door = Door(gameStage.door.x, gameStage.door.y, gameStage.door.orientation);
    game.door = door;

    linkPlatforms(&gameStage, &game.platforms, &game.spinConnections);
    setupLasers(&gameStage, &platformsById);
}


/* ======================== *
*     REGION - LEVELS      *
* ======================== */



GameStage level11() {
    std::vector<PlatformTemplate> platforms {
        PlatformTemplate(1, 2, 1, 2, true, dir_up, -1, 1),
        PlatformTemplate(4, 2, 1, 2, true, dir_left, 1),
        PlatformTemplate(5, 0, 0, 0, true, dir_up, 1),
        PlatformTemplate(7, 2, 1, 0, true, dir_down, 2),
        PlatformTemplate(0, 6, 2, 1, true, dir_right, 2, 2),
        PlatformTemplate(1, -2, 1, 1, true, dir_left, -1, -1, 1),
        PlatformTemplate(1, -6, 1, 1, true, dir_left, -1, -1, -1, 1),
        PlatformTemplate(-2, 0, 1, 1, true, dir_right, -1, -1, -1, 1, true),
        PlatformTemplate(-4, 2, 1, 1, true, dir_left, -1, 3, -1, -1),
        PlatformTemplate(-2, 4, 1, 3, false, dir_right, -1, -1, -1, 2),
    };

    std::vector<LaserSourceTemplate> laserSources {
        //LaserSourceTemplate(0, dir_right, 1),
        LaserSourceTemplate(-1, dir_down, 1),
        LaserSourceTemplate(-1, dir_down, 3),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(1, dir_down, 1, 1),
        LaserTargetTemplate(1, dir_down, 3, 2),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    double zoom = 0.9;

    return GameStage(platforms, player, door, zoom, laserSources, laserTargets);
}


GameStage level12() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 3, true, dir_up, 1, 2),
        PlatformTemplate(5, -1, 3, 1, true, dir_up, 1, 1),
        PlatformTemplate(5, 2, 0, 0, true, dir_up, 1, 1),
        PlatformTemplate(-2, 4, 3, 3, true, dir_up, 1, 1),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    double zoom = 0.9;

    return GameStage(platforms, player, door, zoom);
}


GameStage level1() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 1, 1, 1, false, dir_up),
        PlatformTemplate(5, 1, 1, 1, false, dir_up),
    };

    PlayerTemplate player(0, 0, dir_up);
    DoorTemplate door(6, 0, dir_up);
    double zoom = 1.2;

    return GameStage(platforms, player, door, zoom);
}


GameStage level2() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 2, false, dir_up),
        PlatformTemplate(2, 4, 0, 1, false, dir_up),
        PlatformTemplate(5, 1, 1, 1, true, dir_up),
        PlatformTemplate(5, 4, 1, 1, true, dir_left),
    };

    PlayerTemplate player(0, 0, dir_up);
    DoorTemplate door(2, 3, dir_up);
    double zoom = 1.0;

    return GameStage(platforms, player, door, zoom);
}


GameStage level3() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 2, true, dir_up),
        PlatformTemplate(4, 2, 1, 2, true, dir_left),
        PlatformTemplate(5, 0, 0, 0, true, dir_up),
        PlatformTemplate(7, 2, 1, 0, true, dir_down),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    double zoom = 0.9;

    return GameStage(platforms, player, door, zoom);
}

GameStage level4() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 0, 1, 1, false, dir_up),
        PlatformTemplate(2, 2, 1, 1, true, dir_up),
        PlatformTemplate(0, 4, 1, 1, true, dir_right),
        PlatformTemplate(4, 4, 1, 1, true, dir_left),
        PlatformTemplate(0, 0, 0, 0, true, dir_down),
        PlatformTemplate(4, 0, 0, 0, true, dir_down),
    };

    PlayerTemplate player(1, 1, dir_up);
    DoorTemplate door(2, 1, dir_down);
    double zoom = 1.0;
    
    return GameStage(platforms, player, door, zoom);
}



GameStage level5() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 3, 0, 2, true, dir_up),
        PlatformTemplate(2, 6, 1, 0, true, dir_up),
        PlatformTemplate(2, 8, 0, 0, true, dir_right),
        PlatformTemplate(4, 1, 1, 0, true, dir_left),
    };

    PlayerTemplate player(2, 2, dir_up);
    DoorTemplate door(3, 0, dir_up);
    double zoom = 0.8;

    return GameStage(platforms, player, door, zoom);
}


GameStage level6() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(3, 1, 0, 0, true, dir_right),
        PlatformTemplate(3, 4, 0, 0, true, dir_up),
        PlatformTemplate(4, 3, 0, 0, true, dir_down),
        PlatformTemplate(5, 2, 0, 0, true, dir_up),
        PlatformTemplate(5, 5, 0, 0, true, dir_down),
        PlatformTemplate(2, 6, 0, 2, true, dir_right),
        PlatformTemplate(1, 5, 0, 0, true, dir_right),
    };

    PlayerTemplate player(3, 0, dir_up);
    DoorTemplate door(3, 7, dir_down);
    double zoom = 0.9;
    
    return GameStage(platforms, player, door, zoom);
}

GameStage level7() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(3, 2, 1, 1, true, dir_up),
        PlatformTemplate(0, 4, 1, 1, true, dir_right),
        PlatformTemplate(4, 3, 0, 3, true, dir_up),
        PlatformTemplate(3, 6, 0, 2, true, dir_left),
        PlatformTemplate(6, 6, 0, 1, true, dir_up),
    };

    PlayerTemplate player(3, 0, dir_up);
    DoorTemplate door(5, 7, dir_left);
    double zoom = 0.9;

    return GameStage(platforms, player, door, zoom);
}


GameStage level8() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 2, 1, 1, true, dir_up),
        PlatformTemplate(4, 1, 0, 0, true, dir_right),
        PlatformTemplate(3, 5, 2, 2, true, dir_up),
        PlatformTemplate(6, 6, 1, 1, true, dir_right),
        PlatformTemplate(5, 8, 1, 1, true, dir_down),
        PlatformTemplate(8, 7, 1, 1, true, dir_up),
        PlatformTemplate(3, 9, 0, 0, true, dir_left),
        PlatformTemplate(5, 11, 1, 1, true, dir_right),
        PlatformTemplate(9, 9, 2, 2, false, dir_up),
        PlatformTemplate(7, 0, 1, 1, true, dir_down),
    };

    PlayerTemplate player(3, 4, dir_up);
    DoorTemplate door(6, 12, dir_right);
    double zoom = 0.7;
    
    return GameStage(platforms, player, door, zoom);
}


GameStage level9() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 3, 1, 1, true, dir_right),
        PlatformTemplate(5, 2, 1, 1, true, dir_right),
        PlatformTemplate(5, 4, 1, 1, false, dir_up),
        PlatformTemplate(7, 4, 0, 3, true, dir_up),
        PlatformTemplate(13, 2, 1, 1, true, dir_down),
        PlatformTemplate(15, 4, 1, 1, true, dir_right),
        PlatformTemplate(18, 8, 1, 1, true, dir_right),
        PlatformTemplate(13, 9, 1, 1, true, dir_up),
        PlatformTemplate(11, 12, 1, 1, true, dir_up),
        PlatformTemplate(7, 13, 1, 1, true, dir_left),
        PlatformTemplate(6, 6, 2, 2, false, dir_right),
        PlatformTemplate(2, 14, 1, 1, true, dir_up),
        PlatformTemplate(3, 12, 1, 1, true, dir_down),
        PlatformTemplate(5, 10, 1, 1, true, dir_down),
        PlatformTemplate(4, 6, 1, 1, false, dir_right),
        PlatformTemplate(4, 8, 0, 1, true, dir_down)
    };

    PlayerTemplate player(8, 2, dir_up);
    DoorTemplate door(5, 6, dir_right);
    double zoom = 0.7;
    
    return GameStage(platforms, player, door, zoom);
}


GameStage level10() {
    std::vector<PlatformTemplate> platforms {
        PlatformTemplate(2, 1, 0, 0, true, dir_down),
        PlatformTemplate(3, 2, 1, 1, true, dir_right),
        PlatformTemplate(5, 1, 1, 1, true, dir_up),
        PlatformTemplate(1, 5, 2, 0, true, dir_down),
        PlatformTemplate(5, 4, 1, 1, true, dir_left),
        PlatformTemplate(7, 3, 1, 1, true, dir_up),
        PlatformTemplate(9, 2, 0, 2, true, dir_right),
        PlatformTemplate(11, 5, 0, 2, true, dir_left),
        PlatformTemplate(12, 6, 0, 0, false, dir_left),
        PlatformTemplate(9, 6, 2, 0, true, dir_up),
        PlatformTemplate(1, 8, 0, 1, true, dir_left),
        PlatformTemplate(5, 7, 1, 1, true, dir_up),
        PlatformTemplate(3, 9, 1, 1, true, dir_down),
        PlatformTemplate(6, 10, 2, 0, true, dir_right),
        PlatformTemplate(8, 10, 0, 0, true, dir_left),
        PlatformTemplate(10, 1, 0, 0, true, dir_up),
    };

    PlayerTemplate player(5, 0, dir_up);
    DoorTemplate door(11, 1, dir_up);
    double zoom = 0.9;

    return GameStage(platforms, player, door, zoom);
}



/* ======================== *
*   REGION - LEVELS - END  *
* ======================== */

GameStage getStage(int stage) {
    switch (stage) {
    case 1: return level1();
    case 2: return level2();
    case 3: return level3();
    case 4: return level4();
    case 5: return level5();
    case 6: return level6();
    case 7: return level7();
    case 8: return level8();
    case 9: return level9();
    case 10: return level10();
    case 11: return level11();
    case 12: return level12();
    }

    return level1();
}


void initialiseStage(int stage) {
    game.currentStage = stage;
    initialiseFromStageObject(getStage(stage));
}