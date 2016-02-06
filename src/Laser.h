#ifndef LASER_H_
#define LASER_H_

#include <SFML/Graphics.hpp>
class LaserSource;
class LaserTarget;
#include "IGameObject.h"

class Laser : IGameObject {
public:
    sf::RectangleShape shape;

    int x1, y1, x2, y2;
    int orientation;
    bool active;
    int endX;
    int endY;
    LaserSource* source;
    LaserTarget* target;

    Laser();
    Laser(LaserSource* source);

    void draw();
    void update(Keyboard k);
    void setup(int orientation, int sx, int sy, int ex, int ey);

protected:
    int targetX, targetY;
    int particleTimer;
};

#endif