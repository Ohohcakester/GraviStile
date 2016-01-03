#include "SwitchConnection.h"
#include "LaserTarget.h"
#include "Platform.h"

SwitchConnection::SwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots) :
    laserTarget(laserTarget), connectedPlatforms(platforms), connectedPivots(pivots) {}

void SwitchConnection::flip() {
    isOn = !isOn;
    for (size_t i = 0, n = connectedPivots.size(); i < n; ++i) {
        connectedPivots[i]->toggleRotationDisabledStatus();
    }
    for (size_t i = 0, n = connectedPlatforms.size(); i < n; ++i) {
        connectedPlatforms[i]->toggleDisabledStatus();
    }
}

void SwitchConnection::refresh() {
    if (laserTarget->isOn != this->isOn) {
        flip();
    }
}


// Creates a new switch connection. Must be deleted later on.
SwitchConnection* createSwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots) {
    SwitchConnection* connection = new SwitchConnection(laserTarget, platforms, pivots);
    laserTarget->connection = connection;
    return connection;
}