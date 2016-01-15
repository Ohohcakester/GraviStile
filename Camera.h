#ifndef CAMERA_H_
#define CAMERA_H_

class Player;
class Keyboard;
#include "IGameObject.h"

class Camera : public IGameObject {
public:
    float rotateSpeed;
    float snapSpeed;
    float snapSpeedRotating;

    float zoom;
    float px, py;
    float cx, cy;
    float angle;
    float targetAngle;
    int orientation;
    bool rotating;
    Player* player;

    Camera();

    Camera(Player* player);

    void toRel(float* _x, float* _y);

    void draw();

    void update(Keyboard k);

    void updateAngle();
};

#endif