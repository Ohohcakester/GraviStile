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
#include <iostream>
#include "gamemath.h"

const int DIM_X1 = -20;
const int DIM_X2 = 20;
const int DIM_Y1 = -5;
const int DIM_Y2 = 0;

void LaserSource::initialise() {
    shape = sf::RectangleShape();
    shape.setFillColor(textures->laserSourceColor);
}

void LaserSource::setupLaser(Laser* laser) {
    this->laser = laser;
}

void LaserSource::temporarilyTurnOffLaser() {
    this->laser->active = false;
} 

void LaserSource::fireLaser() {
    int dx, dy;
    orientationToMoveDirection(this->orientation, &dx, &dy);
    int currX = this->cx + dx;
    int currY = this->cy + dy;
    
    int range = 60;
    Grid* grid = &game.grid;
    while (range > 0 && !grid->isBlocked(currX, currY)) {
        currX += dx;
        currY += dy;
        --range;
    }

    laser->setup(this->orientation, this->cx, this->cy, currX, currY);

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
    int x1 = DIM_X1;
    int x2 = DIM_X2;
    int y1 = DIM_Y1;
    int y2 = DIM_Y2;

    float tlx, tly, blx, bly, brx, bry;
    this->generateActualCorners(x1, y1, x2, y2, &tlx, &tly, &blx, &bly, &brx, &bry);
    drawRectangle(&shape, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry);
}

void LaserSource::update(Keyboard k) {

}

bool LaserSource::isWithinClickHitbox(int sx, int sy) {
    float x1 = DIM_X1;
    float x2 = DIM_X2;
    float y1 = DIM_Y1;
    float y2 = DIM_Y2;

    this->toActualCoordindates(&x1, &y1);
    this->toActualCoordindates(&x2, &y2);

    x1 += x;
    x2 += x;
    y1 += y;
    y2 += y;

    return isWithinRectSafe(sx, sy, x1, y1, x2, y2);
}