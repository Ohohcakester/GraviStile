#include <iostream>
#include "globals.h"
#include "GameGlobals.h"
#include "Player.h"
#include "Camera.h"
#include "gamemath.h"

const float ROTATE_SNAP_SPEED = 0.15f;

Camera::Camera() {}

Camera::Camera(Player* player) {
    zoom = global::game.zoom;
    snapSpeed = 0.2f;
    snapSpeedRotating = 0.6f;

    this->player = player;
    this->px = player->x;
    this->py = player->y;
    this->angle = player->angle;
    this->targetAngle = 0;
}

void Camera::update(Keyboard k) {
    float targetX = player->x;
    float targetY = player->y;
    if (player->isRotating) {
        player->getSpriteCoordinates(&targetX, &targetY);
    }

    float dx = targetX - px;
    float dy = targetY - py;
    px += dx*snapSpeed;
    py += dy*snapSpeed;

    updateAngle();
}

void Camera::updateAngle() {
    float diff = clampedAngle(player->angle - angle);
    angle = clampedAngle(angle + diff*ROTATE_SNAP_SPEED);
}