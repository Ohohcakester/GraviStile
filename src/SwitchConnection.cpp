#include "SwitchConnection.h"
#include "LaserTarget.h"
#include "Platform.h"
#include "IGameObject.h"
#include "globals.h"
#include "Textures.h"
#include "AbstractAttachedObject.h"

SwitchConnection::SwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots) :
        laserTarget(laserTarget), connectedPlatforms(platforms), connectedPivots(pivots) {
    setupWires();
}

void SwitchConnection::setupWires()  {
    for (size_t i = 0, n = connectedPlatforms.size(); i < n; ++i) {
        float ex = connectedPlatforms[i]->x;
        float ey = connectedPlatforms[i]->y;

        wires.push_back(SwitchConnectionWire(laserTarget, ex, ey));
    }

    for (size_t i = 0, n = connectedPivots.size(); i < n; ++i) {
        float ex = connectedPivots[i]->x;
        float ey = connectedPivots[i]->y;

        wires.push_back(SwitchConnectionWire(laserTarget, ex, ey));
    }
}

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

void SwitchConnection::draw() {
    for (size_t i = 0, n = wires.size(); i < n; ++i) {
        wires[i].draw();
    }
}


// Creates a new switch connection. Must be deleted later on.
SwitchConnection* createSwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots) {
    SwitchConnection* connection = new SwitchConnection(laserTarget, platforms, pivots);
    laserTarget->connection = connection;
    return connection;
}


// SwitchConnectionWire

SwitchConnectionWire::SwitchConnectionWire(AbstractAttachedObject* laserTarget, float ex, float ey) : ex(ex), ey(ey), laserTarget(laserTarget) {
    sx = laserTarget->x;
    sy = laserTarget->y;
    shape = sf::RectangleShape();
    shape.setFillColor(global::textures->switchConnectionWireColor);
}

void SwitchConnectionWire::update(Keyboard k) {
}

void SwitchConnectionWire::draw() {
    float dsx = 0;
    float dsy = 7.5f;
    laserTarget->toActualCoordindates(&dsx, &dsy);
    drawLine(&shape, sx+dsx, sy+dsy, ex, ey, 2);
}