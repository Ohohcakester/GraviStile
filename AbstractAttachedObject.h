#ifndef ABSTRACT_ATTACHED_OBJECT_H_
#define ABSTRACT_ATTACHED_OBJECT_H_

class Platform;
#include "IGameObject.h"

class AbstractAttachedObject : IGameObject {
public:
    int orientation; // relative to platform.
    int position; // negative: left, positive: right. Only matters for dir_up and dir_down.
    Platform* platform;

    explicit AbstractAttachedObject(Platform* platform, int orientation);

    explicit AbstractAttachedObject(Platform* platform, int orientation, int position);

    virtual void initialise();
};


#endif