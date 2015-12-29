
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

void Camera::rotateTo(int newOrientation, int pivotX, int pivotY) {
    this->cx = pivotX;
    this->cy = pivotY;

    int diff = newOrientation - orientation;
    if (diff >= 3) diff -= 4;
    if (diff < -1) diff += 4;
    orientation = newOrientation;
    //std::cout << diff;
    targetAngle += M_PI / 2 * diff;
    setIsRotating(true);
}

void Camera::setIsRotating(bool value) {
    rotating = value;
    player->setIsRotating(value);
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
    if (!rotating) {
        float dx = player->x - px;
        float dy = player->y - py;
        px += dx*snapSpeed;
        py += dy*snapSpeed;
        return;
    }

    //std::cout << angle << " " << targetAngle << "\n";
    if (angle < targetAngle) {
        angle += rotateSpeed;
        if (angle >= targetAngle) {
            onReach();
        }
    }
    else {
        angle -= rotateSpeed;
        if (angle < targetAngle) {
            onReach();
        }
    }

    float dx = player->x - cx;
    float dy = player->y - cy;
    float remAng = targetAngle - angle;
    float tx = dx*cos(remAng) + dy*sin(remAng) + cx;
    float ty = -dx*sin(remAng) + dy*cos(remAng) + cy;
    dx = tx - px;
    dy = ty - py;

    px += dx*snapSpeedRotating;
    py += dy*snapSpeedRotating;
}

void Camera::onReach() {
    while (targetAngle < 0) targetAngle += 2 * M_PI;
    while (targetAngle > M_PI) targetAngle -= 2 * M_PI;
    angle = targetAngle;
    setIsRotating(false);
}
