#ifndef SWITCH_CONNECTION_H
#define SWITCH_CONNECTION_H

#include <vector>
class LaserTarget;
class Platform;

class SwitchConnection {
private:
    void flip();

public:
    LaserTarget* laserTarget;
    std::vector<Platform*> connectedPivots;
    std::vector<Platform*> connectedPlatforms;

    bool isOn;

    SwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots);

    void refresh();

};

SwitchConnection* createSwitchConnection(LaserTarget* laserTarget, std::vector<Platform*> platforms, std::vector<Platform*> pivots);

#endif