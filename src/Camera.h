#ifndef CAMERA_H_
#define CAMERA_H_

class Player;
class Keyboard;
#include "AbstractCamera.h"

class Camera : public AbstractCamera {
public:
    float snapSpeed;
    float snapSpeedRotating;

    float targetAngle;
    Player* player;

    Camera();

    Camera(Player* player);

    void update(Keyboard k);

    void updateAngle();
};

#endif