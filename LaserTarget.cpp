#include "LaserTarget.h"
#include "Textures.h"
#include "globals.h"
#include "gamemath.h"
#include <iostream>

const int DIM_X1 = -20;
const int DIM_X2 = 20;
const int DIM_Y1 = -5;
const int DIM_Y2 = 10;
const int DIM_Y2_ALT = 30;

void LaserTarget::initialise() {
    shape = sf::RectangleShape();
    shape.setFillColor(textures->laserTargetColor);
}

void LaserTarget::resetTargetOnStatus() {
    currentLaser = nullptr;
    isOn = false;
}

void LaserTarget::laserConnect(Laser* laser) {
    currentLaser = laser;
    isOn = true;
}

void LaserTarget::draw() {
    int x1 = DIM_X1;
    int x2 = DIM_X2;
    int y1 = DIM_Y1;
    int y2 = DIM_Y2;
    if (isOn) y2 = DIM_Y2_ALT;

    float tlx, tly, blx, bly, brx, bry;
    this->generateActualCorners(x1, y1, x2, y2, &tlx, &tly, &blx, &bly, &brx, &bry);
    drawRectangle(&shape, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry);
}
void LaserTarget::update(Keyboard k) {

}

bool LaserTarget::isWithinClickHitbox(int sx, int sy) {
    float x1 = DIM_X1;
    float x2 = DIM_X2;
    float y1 = DIM_Y1;
    float y2 = DIM_Y2;
    if (isOn) y2 = DIM_Y2_ALT;

    this->toActualCoordindates(&x1, &y1);
    this->toActualCoordindates(&x2, &y2);

    x1 += x;
    x2 += x;
    y1 += y;
    y2 += y;

    return isWithinRectSafe(sx, sy, x1, y1, x2, y2);
}