#include <iostream>
#include "AbstractAttachedObject.h"
#include "Platform.h"
#include "gamemath.h"
#include "globals.h"

AbstractAttachedObject::AbstractAttachedObject(Platform* platform, int orientation) : AbstractAttachedObject(platform, orientation, 0) {}

AbstractAttachedObject::AbstractAttachedObject(Platform* platform, int relativeOrientation, int position) :
platform(platform), relativeOrientation(relativeOrientation), position(position) {
    adjustPosition();
    gridToActual(platform->cx, platform->cy, &this->x, &this->y);
}

void AbstractAttachedObject::adjustPosition() {
    switch (relativeOrientation) {
    case dir_up:
    case dir_down:
    {
        if (position < 0) {
            if (-position > platform->leftTiles) {
                std::cout << "Attached Object position error: " << position << ", " << relativeOrientation << " at platform (" << platform->cx << ", " << platform->cy << ")\n";
                position = -platform->leftTiles;
            }
        }
        else if (position > 0) {
            if (position > platform->rightTiles) {
                std::cout << "Attached Object position error: " << position << ", " << relativeOrientation << " at platform (" << platform->cx << ", " << platform->cy << ")\n";
                position = platform->rightTiles;
            }
        }
        break;
    }
    case dir_left: {
        position = -platform->leftTiles;
        break;
    }
    case dir_right: {
        position = platform->rightTiles;
        break;
    }
    }
}

void AbstractAttachedObject::updateActualPosition() {
    this->orientation = (relativeOrientation + platform->orientation) % 4;

    int rx, ry;
    switch (platform->orientation) {
    case dir_up:
        rx = position;
        ry = 0;
        break;
    case dir_right:
        rx = 0;
        ry = position;
        break;
    case dir_down:
        rx = -position;
        ry = 0;
        break;
    case dir_left:
        rx = 0;
        ry = -position;
        break;
    }

    this->cx = platform->cx + rx;
    this->cy = platform->cy + ry;
    //std::cout << platform->cx << " " << platform->cy << " " << " << PLAT \n";
    //std::cout << this->cx << " " << this->cy << " " << this->orientation << "\n";
}


void AbstractAttachedObject::toActualCoordindates(float* dx, float* dy) {
    // Note dx and dy are relative to the origin of the platform.

    // 1. Move to top of tile
    *dy -= TILE_WIDTH / 2;
    // 2. Rotate about tile (0,0)
    rotateVector(dx, dy, orientationToAngle(this->relativeOrientation));
    // 3. Move to the correct tile on the platform. (assuming platform is dir_up facing.)
    *dx += TILE_WIDTH * position;
    // 4. Rotate to the platform's angle.
    rotateVector(dx, dy, platform->angle);
}

// x1,y1 is the top left, x2,y2 is the bottom right.
void AbstractAttachedObject::generateActualCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry) {
    *tlx = x1;
    *tly = y1;
    *blx = x1;
    *bly = y2;
    *brx = x2;
    *bry = y2;

    toActualCoordindates(tlx, tly);
    toActualCoordindates(blx, bly);
    toActualCoordindates(brx, bry);
}
