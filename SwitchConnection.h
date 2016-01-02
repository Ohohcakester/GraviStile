#ifndef SWITCH_CONNECTION_H
#define SWITCH_CONNECTION_H

#include <vector>
class LaserSource;
class Platform;

class SwitchConnection {
private:
    void flip();

public:
    LaserSource* laserSource;
    std::vector<Platform*> connectedPivots;
    std::vector<Platform*> connectedPlatforms;

    bool isOn;

    void refresh();

};



#endif