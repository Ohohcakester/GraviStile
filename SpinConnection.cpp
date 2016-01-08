
#include "IGameObject.h"
#include "SpinConnection.h"
#include "Platform.h"
#include "globals.h"
#include "Textures.h"
class Keyboard;


SpinConnection::SpinConnection() {
    _isNull = true;
}

SpinConnection::SpinConnection(std::vector<Platform*> platforms): platforms(platforms) {
    _isNull = false;
    setupWires();
}

void SpinConnection::setupWires() {
    int n = platforms.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            float sx = platforms[i]->x;
            float sy = platforms[i]->y;
            float ex = platforms[j]->x;
            float ey = platforms[j]->y;

            wires.push_back(SpinConnectionWire(sx, sy, ex, ey));
        }
    }
}

bool SpinConnection::isNull() {
    return _isNull;
}

void SpinConnection::revertToPreviousOrientation() {
    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        platforms[i]->revertToPreviousOrientation();
    }
}

void SpinConnection::rotateBy(int oldOrientation, int newOrientation) {
    int difference = newOrientation - oldOrientation + 4;
    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        platforms[i]->saveCurrentOrientation();

        int newOrientation = (platforms[i]->orientation + difference) % 4;
        platforms[i]->setOrientation(newOrientation);
    }
}

void SpinConnection::finishRotation() {
    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        platforms[i]->finishRotation();
    }
}

void SpinConnection::draw() {
    for (size_t i = 0, n = wires.size(); i < n; ++i) {
        wires[i].draw();
    }
}

// Creates a new spin connection. Must be deleted later on.
SpinConnection* createSpinConnection(std::vector<Platform*> platforms) {
    SpinConnection* connection = new SpinConnection(platforms);
    for (size_t i = 0; i < platforms.size(); ++i) {
        platforms[i]->spinConnection = connection;
    }
    return connection;
}


// SpinConnectionWire

SpinConnectionWire::SpinConnectionWire(float sx, float sy, float ex, float ey) : sx(sx), sy(sy), ex(ex), ey(ey) {
    shape = sf::RectangleShape();
    shape.setFillColor(textures->spinConnectionWireColor);
}

void SpinConnectionWire::update(Keyboard k) {
}

void SpinConnectionWire::draw() {
    drawLine(&shape, sx, sy, ex, ey, 2.5f);
}