#include <iostream>
#include "AbstractAttachedObject.h"
#include "Platform.h"

AbstractAttachedObject::AbstractAttachedObject(Platform* platform, int orientation) : AbstractAttachedObject(platform, orientation, 0) {}

AbstractAttachedObject::AbstractAttachedObject(Platform* platform, int relativeOrientation, int position) :
platform(platform), relativeOrientation(relativeOrientation), position(position) {
    adjustPosition();
    initialise();
}

void AbstractAttachedObject::initialise() {}

void AbstractAttachedObject::adjustPosition() {
    switch (relativeOrientation) {
    case dir_up:
    case dir_down:
    {
        if (position < 0) {
            if (-position > platform->leftTiles) {
                std::cout << "Attached Object position error: " << position << ", " << relativeOrientation << " at platform (" << platform->cx << ", " << platform->cy;
                position = -platform->leftTiles;
            }
        }
        else if (position > 0) {
            if (position > platform->rightTiles) {
                std::cout << "Attached Object position error: " << position << ", " << relativeOrientation << " at platform (" << platform->cx << ", " << platform->cy;
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
}