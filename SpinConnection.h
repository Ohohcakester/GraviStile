#ifndef SPIN_CONNECTION_H_
#define SPIN_CONNECTION_H_

#include "IGameObject.h"
#include <vector>
class Platform;
class SpinConnectionWire;

class SpinConnection {
public:
    bool _isNull;
    std::vector<Platform*> platforms;
    std::vector<SpinConnectionWire> wires;

    SpinConnection();

    SpinConnection(std::vector<Platform*> platforms);
    void setupWires();

    bool isNull();
    void rotateBy(int oldOrientation, int newOrientation);
    void revertToPreviousOrientation();
    void finishRotation();
    void draw();
};

class SpinConnectionWire : IGameObject {
    float sx, sy, ex, ey;
    sf::RectangleShape shape;

public:
    SpinConnectionWire(float sx, float sy, float ex, float ey);
    void draw();
    void update(Keyboard k);
};


SpinConnection* createSpinConnection(std::vector<Platform*> platforms);

#endif