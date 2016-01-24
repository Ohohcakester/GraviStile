#include "AbstractCamera.h"
#include "globals.h"

AbstractCamera::AbstractCamera() {
    zoom = 0.4f;
    px, py = 0;
    angle = 0;
}

void AbstractCamera::toRel(float* _x, float* _y) {
    float dx = *_x - px;
    float dy = *_y - py;
    float theta = angle - atan2(dy, dx);
    float length = sqrt(dx*dx + dy*dy);

    *_x = zoom*length*cos(theta) + RES_X / 2;
    *_y = RES_Y / 2 - zoom*length*sin(theta);
}

void AbstractCamera::toAbs(float* _x, float* _y) {
    *_x = (*_x - RES_X / 2)/zoom;
    *_y = (RES_Y / 2 - *_y)/zoom;

    float length = sqrt(*_x**_x + *_y**_y);
    float theta = angle - atan2(*_y, *_x);
    
    *_x = px + length * cos(theta);
    *_y = py + length * sin(theta);
}

void AbstractCamera::draw() {

}