#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "gameobjects.h"
#include "keyboard.h"
#include "globals.h"


void IGameObject::drawCircle(sf::CircleShape shape, int px, int py) {
    px = game.camera.toRelX(px);
    py = game.camera.toRelY(py);
    shape.setOrigin(-px+shape.getRadius(),-py+shape.getRadius());
    window.draw(shape);
}


void IGameObject::drawRectangle(sf::RectangleShape shape, int tl_x, int tl_y, int bl_x, int bl_y, int br_x, int br_y) {
    tl_x = game.camera.toRelX(tl_x);
    tl_y = game.camera.toRelY(tl_y);
    bl_x = game.camera.toRelX(bl_x);
    bl_y = game.camera.toRelY(bl_y);
    br_x = game.camera.toRelX(br_x);
    br_y = game.camera.toRelY(br_y);

    float dx1 = tl_x-bl_x;
    float dy1 = tl_y-bl_y;
    float dx2 = br_x-bl_x;
    float dy2 = br_y-bl_y;
    float width = sqrt(dx2*dx2+dy2*dy2);
    float height = sqrt(dx1*dx1+dy1*dy1);
    float angle = atan2(dy2,dx2)*180/M_PI;

    shape.setSize(sf::Vector2f(width,height));
    shape.setPosition(tl_x,tl_y);
    shape.setRotation(angle);
    window.draw(shape);
}




Player::Player() {
    x = 0; y = 0;
    speed = 10;
    jumpSpeed = 11;
    gravity = 0.4f;
    shape = sf::CircleShape(50);
    shape.setFillColor(sf::Color::Green);
}

void Player::draw() {
    drawCircle(shape, x, y);
}

void Player::update(Keyboard k) {
    vx = 0;
    if (currentPlatform.isNull) vy += gravity;
    if (k.left) vx -= speed;
    if (k.right) vx += speed;
 
    x += vx;
    y += vy;
}

void Player::getGridCoordinates(int* gridX, int* gridY) {
    actualToGrid(x, y, gridX, gridY);
}

void Player::jump() {
    currentPlatform = Platform(); // isNull
    std::cout<<"jumping\n";
    vy = (-1) * speed;
}


Platform::Platform() {
    this->isNull = true;
}


Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, bool rotatable, int orientation) {
    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->leftTiles = leftTiles;
    this->rightTiles = rightTiles;
    this->rotatable = rotatable;
    this->orientation = orientation;

    shape = sf::RectangleShape();
    shape.setFillColor(sf::Color::Green);
}

void Platform::draw() {
    drawRectangle(shape,30,40,30,80,130,80);
}

void Platform::update(Keyboard k) {
}

Door::Door() {
    this->isNull = true;
}

Door::Door(int cx, int cy, int orientation) {
    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->orientation = orientation;
}

void Door::draw() {
    
}

void Door::update(Keyboard k) {

}


Camera::Camera() {}

Camera::Camera(Player* player) {
    rotateSpeed = 0.1f;
    snapSpeed = 0.2f;

    this->player = player;
    this->px = player->x;
    this->py = player->y;
}

void Camera::rotateTo(int newOrientation) {
    int diff = newOrientation - orientation;
    if (diff >= 3) diff -= 3;
    if (diff < -1) diff += 3;
    orientation = newOrientation;
    targetAngle += M_PI/2 * diff;
    rotating = true;
}

float Camera::toRelX(float _x) {
    return _x - px + RES_X/2;
}

float Camera::toRelY(float _y) {
    return _y - py + RES_Y/2;
}

void Camera::draw() {

}

void Camera::update(Keyboard k) {
    float dx = player->x - px;
    float dy = player->y - py;
    px += dx*snapSpeed;
    py += dy*snapSpeed;

    if (!rotating) return;
    if (angle < targetAngle) {
        angle += rotateSpeed;
        if (angle >= targetAngle) {
            onReach();
        }
    } else {
        angle -= rotateSpeed;
        if (angle < targetAngle) {
            onReach();
        }
    }
}

void Camera::onReach() {
    while (targetAngle < 0) targetAngle += 2*M_PI;
    while (targetAngle > M_PI) targetAngle -= 2*M_PI;
    angle = targetAngle;
    rotating = false;
}