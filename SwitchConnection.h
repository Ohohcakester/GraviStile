#ifndef SWITCH_CONNECTION_H
#define SWITCH_CONNECTION_H

#include "IGameObject.h"
#include <vector>
class LaserTarget;
class AbstractAttachedObject;
class Platform;
class SwitchConnectionWire;

class SwitchConnection {
private:
    void flip();

public:
    LaserTarget* laserTarget;
    std::vector<Platform*> connectedPivots;
    std::vector<Platform*> connectedPlatforms;
    std::vector<SwitchConnectionWire> wires;

    bool isOn;

    SwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots);
    void setupWires();

    void refresh();
    void draw();
};

class SwitchConnectionWire : IGameObject {
    float sx, sy, ex, ey;
    sf::RectangleShape shape;
    AbstractAttachedObject* laserTarget;

public:
    SwitchConnectionWire::SwitchConnectionWire(AbstractAttachedObject* laserTarget, float ex, float ey);
    void draw();
    void update(Keyboard k);
};

SwitchConnection* createSwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots);

#endif