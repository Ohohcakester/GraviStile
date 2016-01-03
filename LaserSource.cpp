#include "LaserSource.h"
#include "LaserTarget.h"
#include "AbstractAttachedObject.h"
#include "Platform.h"
#include "Laser.h"
#include "Orientations.h"
#include "globals.h"
#include "GameGlobals.h"
#include "Grid.h"
#include "Textures.h"
#include "globals.h"

void LaserSource::initialise() {
    shape = sf::RectangleShape();
    shape.setFillColor(textures->laserSourceColor);
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
    int x1 = -40;
    int x2 = 40;
    int y1 = -5;
    int y2 = 0;

    float tlx, tly, blx, bly, brx, bry;
    this->generateActualCorners(x1, y1, x2, y2, &tlx, &tly, &blx, &bly, &brx, &bry);
    drawRectangle(&shape, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry);
}
void LaserSource::update(Keyboard k) {

}
