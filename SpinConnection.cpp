#include "SpinConnection.h"
#include "Platform.h"


SpinConnection::SpinConnection() {
    _isNull = true;
}

SpinConnection::SpinConnection(std::vector<Platform*> platforms): platforms(platforms) {
    _isNull = false;
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


// Creates a new spin connection. Must be deleted later on.
SpinConnection* createSpinConnection(std::vector<Platform*> platforms) {
    SpinConnection* connection = new SpinConnection(platforms);
    for (size_t i = 0; i < platforms.size(); ++i) {
        platforms[i]->spinConnection = connection;
    }
    return connection;
}