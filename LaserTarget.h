#ifndef LASER_TARGET_H_
#define LASER_TARGET_H_

class SwitchConnection;
class Laser;
#include "AbstractAttachedObject.h"

class LaserTarget : AbstractAttachedObject {
public:
    SwitchConnection* connection;
    Laser* currentLaser;
    bool isOn;

    void initialise();

    void resetTargetOnStatus();

    void draw();
    void update(Keyboard k);
};


#endif