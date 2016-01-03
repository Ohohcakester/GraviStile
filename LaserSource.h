#ifndef LASERSOURCE_H_
#define LASERSOURCE_H_

#include <SFML/Graphics.hpp>
class Laser;
class Keyboard;
#include "AbstractAttachedObject.h"

class LaserSource : public AbstractAttachedObject {
public:
    sf::RectangleShape shape;

    Laser* laser;

    LaserSource(Platform* platform, int orientation) : AbstractAttachedObject(platform, orientation) {}
    LaserSource(Platform* platform, int relativeOrientation, int position) : AbstractAttachedObject(platform, relativeOrientation, position) {}

    // override
    void initialise();
    void setupLaser(Laser* laser);

    void temporarilyTurnOffLaser();
    void fireLaser();

    void draw();
    void update(Keyboard k);
};







#endif