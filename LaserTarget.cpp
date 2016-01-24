#include "LaserTarget.h"
#include "Textures.h"
#include "globals.h"

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
    int x1 = -20;
    int x2 = 20;
    int y1 = -5;
    int y2 = 10;
    if (isOn) y2 = 30;

    float tlx, tly, blx, bly, brx, bry;
    this->generateActualCorners(x1, y1, x2, y2, &tlx, &tly, &blx, &bly, &brx, &bry);
    drawRectangle(&shape, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry);
}
void LaserTarget::update(Keyboard k) {

}