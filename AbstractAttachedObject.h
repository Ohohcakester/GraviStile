#ifndef ABSTRACT_ATTACHED_OBJECT_H_
#define ABSTRACT_ATTACHED_OBJECT_H_

class Platform;
#include "IGameObject.h"

class AbstractAttachedObject : IGameObject {
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

    virtual void initialise();
};


#endif