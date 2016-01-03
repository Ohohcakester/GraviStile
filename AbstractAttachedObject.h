#ifndef ABSTRACT_ATTACHED_OBJECT_H_
#define ABSTRACT_ATTACHED_OBJECT_H_

class Platform;
#include "IGameObject.h"

class AbstractAttachedObject : public IGameObject {
private:
    void adjustPosition();

public:
    int relativeOrientation; // relative to platform.
    int position; // negative: left, positive: right. Only matters for dir_up and dir_down.
    Platform* platform;

    int cx;
    int cy;
    int orientation;

    explicit AbstractAttachedObject(Platform* platform, int orientation);

    explicit AbstractAttachedObject(Platform* platform, int orientation, int position);

    void updateActualPosition();

    void toActualCoordindates(float* dx, float* dy);

    void generateActualCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry);
};


#endif