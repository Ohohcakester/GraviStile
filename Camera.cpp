#include <iostream>
#include "globals.h"
#include "Player.h"
#include "Camera.h"

Camera::Camera() {}

Camera::Camera(Player* player) {
    freeze = false;

    zoom = 0.4;
    rotateSpeed = 0.1f;
    snapSpeed = 0.2f;
    snapSpeedRotating = 0.6f;

    this->orientation = 0;
    this->player = player;
    this->px = player->x;
    this->py = player->y;
    this->angle = 0;
    this->targetAngle = 0;
}

void Camera::toRel(float* _x, float* _y) {
    float dx = *_x - px;
    float dy = *_y - py;
    float theta = angle - atan2(dy, dx);
    float length = sqrt(dx*dx + dy*dy);

    *_x = game.zoom*length*cos(theta) + RES_X / 2;
    *_y = RES_Y / 2 - game.zoom*length*sin(theta);
}

void Camera::draw() {

}

void Camera::update(Keyboard k) {
    angle = player->angle;
    float dx = player->x - px;
    float dy = player->y - py;
    px += dx*snapSpeed;
    py += dy*snapSpeed;
}