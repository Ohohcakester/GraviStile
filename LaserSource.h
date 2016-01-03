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

    // override
    void initialise();

    void temporarilyTurnOffLaser();
    void fireLaser();

    void draw();
    void update(Keyboard k);
};







#endif