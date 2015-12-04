#include <SFML/Graphics.hpp>
#include "gameobjects.h"
#include "keyboard.h"
#include "globals.h"


void IGameObject::drawCircle(sf::CircleShape shape, int px, int py) {
    shape.setOrigin(-px+shape.getRadius(),-py+shape.getRadius());
    window.draw(shape);
}




Player::Player() {
    x = 0; y = 0;
    speed = 10;
    shape = sf::CircleShape(50);
    shape.setFillColor(sf::Color::Green);
}

void Player::draw() {
    drawCircle(shape, x, y);
}

void Player::update(Keyboard k) {
    vx = 0; vy = 0;
    if (k.down) vy += speed;
    if (k.up) vy -= speed;
    if (k.left) vx -= speed;
    if (k.right) vx += speed;

    x += vx;
    y += vy;
}

void Player::getGridCoordinates(int* gridX, int* gridY) {
    actualToGrid(x, y, gridX, gridY);
}




Platform::Platform() {
    this->isNull = true;
}

Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, int orientation) {
    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->leftTiles = leftTiles;
    this->rightTiles = rightTiles;
    this->orientation = orientation;
}

void Platform::draw() {

}

void Platform::update(Keyboard k) {
}



Door::Door(int cx, int cy, int orientation) {
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->orientation = orientation;
}

void Door::draw() {
}

void Door::update(Keyboard k) {

}