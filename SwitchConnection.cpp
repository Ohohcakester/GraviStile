#include "SwitchConnection.h"
#include "LaserTarget.h"
#include "Platform.h"

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