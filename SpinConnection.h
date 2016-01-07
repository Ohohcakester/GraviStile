#ifndef SPIN_CONNECTION_H_
#define SPIN_CONNECTION_H_

#include <vector>
class Platform;

class SpinConnection {
public:
    bool _isNull;
    std::vector<Platform*> platforms;

    SpinConnection();

    SpinConnection(std::vector<Platform*> platforms);

    bool isNull();
    void rotateBy(int oldOrientation, int newOrientation);
    void revertToPreviousOrientation();
};

SpinConnection* createSpinConnection(std::vector<Platform*> platforms);

#endif