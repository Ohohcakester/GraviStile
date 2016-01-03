#ifndef LASER_TARGET_H_
#define LASER_TARGET_H_

#include <SFML/Graphics.hpp>
class SwitchConnection;
class Laser;
#include "AbstractAttachedObject.h"

class LaserTarget : public AbstractAttachedObject {
public:
    sf::RectangleShape shape;

    SwitchConnection* connection;
    Laser* currentLaser;
    bool isOn;

    LaserTarget(Platform* platform, int orientation) : AbstractAttachedObject(platform, orientation) {}
    LaserTarget(Platform* platform, int relativeOrientation, int position) : AbstractAttachedObject(platform, relativeOrientation, position) {}

    void initialise();

    void resetTargetOnStatus();
    void laserConnect(Laser* laser);

    void draw();
    void update(Keyboard k);
};


#endif