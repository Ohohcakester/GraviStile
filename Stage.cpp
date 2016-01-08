#include <iostream>
#include <vector>
#include <map>
#include "globals.h"
#include "Stage.h"
#include "SpinConnection.h"
#include "GameGlobals.h"
#include "gameobjects.h"
GameStage getStage(int stage);


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


void initialiseStage(int stage) {
    game.currentStage = stage;
    initialiseFromStageObject(getStage(stage));
}