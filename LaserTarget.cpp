#include "LaserTarget.h"


void LaserTarget::initialise() {

}

void LaserTarget::resetTargetOnStatus() {
    currentLaser = NULL;
    isOn = false;
}

void LaserTarget::laserConnect(Laser* laser) {
    currentLaser = laser;
    isOn = true;
}

void LaserTarget::draw() {

}
void LaserTarget::update(Keyboard k) {

}