#ifndef LASER_TARGET_H_
#define LASER_TARGET_H_

class SwitchConnection;
class Laser;
#include "AbstractAttachedObject.h"

class LaserTarget : public AbstractAttachedObject {
public:
    SwitchConnection* connection;
    Laser* currentLaser;
    bool isOn;

    void initialise();

    void resetTargetOnStatus();
    void laserConnect(Laser* laser);

    void draw();
    void update(Keyboard k);
};


#endif