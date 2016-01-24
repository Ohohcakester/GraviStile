#ifndef LASER_TARGET_H_
#define LASER_TARGET_H_

#include <SFML/Graphics.hpp>
class SwitchConnection;
class Laser;
#include "AbstractAttachedObject.h"
#include "IClickable.h"

class LaserTarget : public AbstractAttachedObject, public IClickable {
public:
    sf::RectangleShape shape;

    SwitchConnection* connection;
    Laser* currentLaser;
    bool isOn;

    void initialise();

    LaserTarget(Platform* platform, int orientation) : AbstractAttachedObject(platform, orientation) {
        initialise();
    }
    LaserTarget(Platform* platform, int relativeOrientation, int position) : AbstractAttachedObject(platform, relativeOrientation, position) {
        initialise();
    }


    void resetTargetOnStatus();
    void laserConnect(Laser* laser);

    void draw();
    void update(Keyboard k);
    bool isWithinClickHitbox(int x, int y);
};


#endif