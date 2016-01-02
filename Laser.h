#ifndef LASER_H_
#define LASER_H_

class LaserSource;
class LaserTarget;
#include "IGameObject.h"

class Laser : IGameObject {
public:
    bool _isNull;

    int x1, y1, x2, y2;
    bool active;
    int endX;
    int endY;
    LaserSource* source;
    LaserTarget* target;

    Laser();
    Laser(LaserSource* source);

    bool isNull();

    void draw();
    void update(Keyboard k);
};

extern Laser* NULL_LASER;


#endif