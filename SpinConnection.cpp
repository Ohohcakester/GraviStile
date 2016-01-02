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

void SpinConnection::rotateBy(int oldOrientation, int newOrientation) {
    int difference = newOrientation - oldOrientation + 4;
    for (int i = 0; i < platforms.size(); ++i) {
        int newOrientation = (platforms[i]->orientation + difference) % 4;
        platforms[i]->setOrientation(newOrientation);
    }
}

// Creates a new spin connection. Must be deleted later on.
SpinConnection* createSpinConnection(std::vector<Platform*> platforms) {
    SpinConnection* connection = new SpinConnection(platforms);
    for (int i = 0; i < platforms.size(); ++i) {
        platforms[i]->spinConnection = connection;
    }
    return connection;
}

SpinConnection* NULL_SPIN_CONNECTION = new SpinConnection();