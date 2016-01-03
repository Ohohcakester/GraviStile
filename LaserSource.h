#ifndef LASERSOURCE_H_
#define LASERSOURCE_H_

class Laser;
class Keyboard;
#include "AbstractAttachedObject.h"

class LaserSource : public AbstractAttachedObject {
public:
    Laser* laser;

    // override
    void initialise();

    void temporarilyTurnOffLaser();
    void fireLaser();

    void draw();
    void update(Keyboard k);
};







#endif