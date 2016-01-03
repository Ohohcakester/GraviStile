
#include "GameGlobals.h"

GameGlobals::GameGlobals() {
    currentStage = -1;
    width = 0;
    height = 0;
    zoom = 1;
    puzzleComplete = false;
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
}

void GameGlobals::onStart() {
    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        platforms[i]->repositionAttachedObjects();
    }

    refreshMapState();
}

void GameGlobals::finishRotatingTrigger() {
    refreshMapState();
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
}
