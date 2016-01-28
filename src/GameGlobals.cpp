#include <iostream>
#include "GameGlobals.h"
#include "globals.h"
#include "Sfx.h"

int REFRESH_FRAMES = 5;

GameGlobals::GameGlobals() {
    refreshCounter = 0;
    currentStage = -1;
    width = 0;
    height = 0;
    zoom = 1;
    puzzleComplete = false;
    camera = nullptr;
}

void GameGlobals::assignNewCamera(AbstractCamera* camera) {
    this->camera = camera;
}

GameGlobals::~GameGlobals() {
    for (size_t i = 0, n = spinConnections.size(); i < n; ++i) {
        delete spinConnections[i];
    }

    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        delete platforms[i];
    }

    for (size_t i = 0, n = switchConnections.size(); i < n; ++i) {
        delete switchConnections[i];
    }

    for (size_t i = 0, n = lasers.size(); i < n; ++i) {
        delete lasers[i];
    }

    for (size_t i = 0, n = laserSources.size(); i < n; ++i) {
        delete laserSources[i];
    }

    for (size_t i = 0, n = laserTargets.size(); i < n; ++i) {
        delete laserTargets[i];
    }

    for (size_t i = 0, n = sfxList.size(); i < n; ++i) {
        delete sfxList[i];
    }

    if (camera != nullptr) {
        delete camera;
    }
}

void GameGlobals::update() {
    if (!player.isRotating) {
        refreshCounter++;
        while (refreshCounter >= REFRESH_FRAMES) {
            refreshCounter -= REFRESH_FRAMES;
            refreshMapState();
        }
    }
}

void GameGlobals::updateAllSfx() {
    for (size_t i = 0, n = sfxList.size(); i < n; ++i) {
        if (sfxList[i]->isActive) sfxList[i]->update(global::key);
    }

    sfxArrayClearCounter--;
    if (sfxArrayClearCounter <= 0) {
        // cleanup inactive sfx from sfx array.
        int current = 0;
        for (size_t i = 0, n = sfxList.size(); i < n; ++i) {
            if (sfxList[i]->isActive) {
                sfxList[current] = sfxList[i];
                current++;
            }
            else {
                delete sfxList[i];
            }
        }
        sfxList.resize(current);

        // Reset timer for clearing sfx.
        sfxArrayClearCounter = 60;
    }
}

void GameGlobals::spawnNewSfx(sfx::Sfx* newSfx) {
    sfxList.push_back(newSfx);
}

void GameGlobals::onStart() {
    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        platforms[i]->repositionAttachedObjects();
    }

    refreshMapState();
    refreshCounter += REFRESH_FRAMES * 5;

    spawnNewSfx(new sfx::LevelName(stageName, currentStage));
}

void GameGlobals::finishRotatingTrigger() {
    refreshMapState();
    refreshCounter += REFRESH_FRAMES * 5;
}

void GameGlobals::refreshMapState() {
    grid.reset();
    grid.addPlatforms(&this->platforms);

    for (size_t i = 0, n = laserTargets.size(); i < n; ++i) {
        laserTargets[i]->resetTargetOnStatus();
    }

    for (size_t i = 0, n = laserSources.size(); i < n; ++i) {
        laserSources[i]->fireLaser();
    }

    for (size_t i = 0, n = switchConnections.size(); i < n; ++i) {
        switchConnections[i]->refresh();
    }

    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        bool unblock = platforms[i]->tryUnblockFromDisabled(&grid, &player);
        if (unblock) {
            grid.addPlatform(platforms[i]);
        }
    }
}
