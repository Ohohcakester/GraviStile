#ifndef ABSTRACT_CAMERA_H_
#define ABSTRACT_CAMERA_H_

class Keyboard;
#include "IGameObject.h"

class AbstractCamera : public IGameObject {
public:
    float zoom;
    float px, py;
    float angle;

    AbstractCamera();

    void toRel(float* _x, float* _y);
    void toAbs(float* _x, float* _y);

    void draw();
};


#endif