#include "LaserSource.h"
#include "LaserTarget.h"
#include "AbstractAttachedObject.h"
#include "Platform.h"
#include "Laser.h"
#include "Orientations.h"
#include "globals.h"
#include "GameGlobals.h"
#include "Grid.h"

void LaserSource::initialise() {

}


void LaserSource::temporarilyTurnOffLaser() {
    this->laser->active = false;
} 

void LaserSource::fireLaser() {
    int dx, dy;
    orientationToMoveDirection(this->orientation, &dx, &dy);
    int currX = this->cx + dx;
    int currY = this->cy + dy;
    
    int range = 30;
    Grid* grid = &game.grid;
    while (range > 0 && !grid->isBlocked(currX, currY)) {
        currX += dx;
        currY += dy;
        --range;
    }

    laser->setup(orientation, this->cx, this->cy, currX, currY);

    for (size_t i = 0, n = game.laserTargets.size(); i < n; ++i) {
        LaserTarget* target = game.laserTargets[i];
        if (areComplementaryOrientations(this->orientation, target->orientation) &&
                currX == target->cx && currY == target->cy) {
            target->laserConnect(this->laser);
            break;
        }
    }
    
    this->laser->active = true;
}

void LaserSource::draw() {

}
void LaserSource::update(Keyboard k) {

}
