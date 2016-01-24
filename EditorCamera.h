#ifndef EDITOR_CAMERA_H_
#define EDITOR_CAMERA_H_

class Keyboard;
#include "AbstractCamera.h"

class EditorCamera : public AbstractCamera {
public:
    EditorCamera();

    void update(Keyboard k);

protected:
    int moveSpeed;
    float zoomSpeed = 0.03f;
};


#endif