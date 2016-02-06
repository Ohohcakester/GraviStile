#include "EditorCamera.h"
#include "keyboard.h"
#include <iostream>

EditorCamera::EditorCamera(int x, int y, float zoom) {
    this->px = x;
    this->py = y;
    this->zoom = zoom;
    this->moveSpeed = 5;
    this->zoomSpeed = 0.03f;
}

void EditorCamera::update(Keyboard k) {
    int vx = 0, vy = 0;

    float scaledMoveSpeed = moveSpeed / zoom;
    if (k.up) vy -= scaledMoveSpeed;
    if (k.down) vy += scaledMoveSpeed;
    if (k.left) vx -= scaledMoveSpeed;
    if (k.right) vx += scaledMoveSpeed;
    if (k.comma) zoom += zoomSpeed;
    if (k.period) zoom -= zoomSpeed;

    px += vx;
    py += vy;
}