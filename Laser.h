#ifndef LASER_H_
#define LASER_H_

class LaserSource;
class LaserTarget;
#include "IGameObject.h"

class Laser : IGameObject {
public:
    int x1, y1, x2, y2;
    bool active;
    int endX;
    int endY;
    LaserSource* source;
    LaserTarget* target;

    Laser();
    Laser(LaserSource* source);

    void draw();
    void update(Keyboard k);
};

#endif