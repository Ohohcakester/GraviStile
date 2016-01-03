#include <iostream>
#include "globals.h"
#include "GameGlobals.h"
#include "Textures.h"
#include "Point.h"
#include "Platform.h"
#include "gamemath.h"
#include "SpinConnection.h"
#include "LaserSource.h"
#include "LaserTarget.h"

Platform::Platform() {
    this->isNull = true;
}

Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, bool rotatable, int orientation, bool isDisabled, bool isRotationDisabled) :
    cx(cx), cy(cy), leftTiles(leftTiles), rightTiles(rightTiles), rotatable(rotatable), orientation(orientation), isRotationDisabled(isRotationDisabled)
{
    rotateSpeed = 0.1f;

    if (isDisabled) this->disabledStatus = platformStatus_disabled;
    else this->disabledStatus = platformStatus_enabled;

    spinConnection = NULL;
    this->isNull = false;
    gridToActual(cx, cy, &this->x, &this->y);

    setOrientation(orientation);
    onReach();

    sprite.setTexture(textures->pivot);
    shape = sf::RectangleShape();
    shape.setFillColor(textures->platformColor);
    extraLineShape = sf::RectangleShape();
    extraLineShape.setFillColor(textures->platformSurfaceColor);
    pivotShape = sf::CircleShape(game.zoom*TILE_WIDTH / 3);
    pivotShape.setFillColor(sf::Color::Magenta);
}

void Platform::draw() {
    float _x1 = - TILE_WIDTH * (leftTiles + 0.5);
    float _x2 = TILE_WIDTH * (rightTiles + 0.5);
    float _y1 = - TILE_WIDTH / 2;
    float _y2 = TILE_WIDTH / 2;

    float tlx, tly, blx, bly, brx, bry;
    generateRotatedCorners(_x1, _y1, _x2, _y2, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
    drawRectangle(&shape, x+tlx, y+tly, x+blx, y+bly, x+brx, y+bry);

    if (this->rotatable) {
        int fraction = 5;
        float _ey2 = _y1 + (_y2 - _y1) / fraction;
        generateRotatedCorners(_x1, _y1, _x2, _ey2, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
        drawRectangle(&extraLineShape, x+tlx, y+tly, x+blx, y+bly, x+brx, y+bry);

        if (!this->isRotationDisabled) {
            float radius = game.zoom*TILE_WIDTH / 3;
            generateRotatedCorners(-radius, -radius, radius, radius, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
            drawSprite(&sprite, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry);
        }
    }
}

void Platform::update(Keyboard k) {
    if (isRotating) {
        float diff = clampedAngularDifference(angle, targetAngle);
        if (diff < 0) {
            angle -= rotateSpeed;
            diff = clampedAngularDifference(angle, targetAngle);
            if (diff >= 0) {
                onReach();
            }
        } else {
            angle += rotateSpeed;
            diff = clampedAngularDifference(angle, targetAngle);
            if (diff < 0) {
                onReach();
            }
        }
    }
}

void Platform::onReach() {
    angle = targetAngle;
    isRotating = false;
    this->repositionAttachedObjects();
}

bool Platform::isDisabled() {
    return disabledStatus != platformStatus_enabled;
}


void Platform::toggleDisabledStatus() {
    switch (disabledStatus) {
    case platformStatus_disabled:
        disabledStatus = platformStatus_waitingForEnable;
        break;
    case platformStatus_waitingForEnable:
        disabledStatus = platformStatus_disabled;
        break;
    case platformStatus_enabled:
        disabledStatus = platformStatus_disabled;
        break;
    }
}

void Platform::toggleRotationDisabledStatus() {
    isRotationDisabled = !isRotationDisabled;
}

void Platform::rotateTo(int newOrientation) {
    if (spinConnection == NULL) {
        setOrientation(newOrientation);
    }
    else {
        spinConnection->rotateBy(orientation, newOrientation);
    }
}

void Platform::setOrientation(int orientation) {
    this->orientation = orientation;

    this->x1 = x - TILE_WIDTH / 2;
    this->x2 = x + TILE_WIDTH / 2;
    this->y1 = y - TILE_WIDTH / 2;
    this->y2 = y + TILE_WIDTH / 2;

    if (orientation == dir_up) {
        this->x1 -= TILE_WIDTH * leftTiles;
        this->x2 += TILE_WIDTH * rightTiles;
    }

    if (orientation == dir_right) {
        this->y1 -= TILE_WIDTH * leftTiles;
        this->y2 += TILE_WIDTH * rightTiles;
    }

    if (orientation == dir_down) {
        this->x1 -= TILE_WIDTH * rightTiles;
        this->x2 += TILE_WIDTH * leftTiles;
    }

    if (orientation == dir_left) {
        this->y1 -= TILE_WIDTH * rightTiles;
        this->y2 += TILE_WIDTH * leftTiles;
    }

    this->targetAngle = orientationToAngle(orientation);
    this->isRotating = true;
    this->temporarilyDisableLaserSources();
}

bool Platform::isUnderDoor(int doorX, int doorY) {
    switch (orientation) {
    case dir_up:
        return (cx - leftTiles <= doorX && doorX <= cx + rightTiles);
    case dir_down:
        return (cx - rightTiles <= doorX && doorX <= cx + leftTiles);
    case dir_left:
        return (cy - rightTiles <= doorY && doorY <= cy + leftTiles);
    case dir_right:
        return (cy - leftTiles <= doorY && doorY <= cy + rightTiles);
    }
    return false;
}

bool Platform::sweep(bool right) {
    int leftQuad;
    int rightQuad;
    if ((right && (orientation == dir_up || orientation == dir_down))
        || ((!right) && (orientation == dir_right || orientation == dir_left))) { // quads24
        switch (orientation) {
        case dir_up:
        case dir_right:
            leftQuad = TILE_WIDTH * leftTiles;
            rightQuad = TILE_WIDTH * rightTiles;
            break;
        case dir_down:
        case dir_left:
            leftQuad = TILE_WIDTH * rightTiles;
            rightQuad = TILE_WIDTH * leftTiles;
            break;
        }
        //std::cout << "right " << right << " orientation " << orientation;
        for (size_t i = 0; i<game.platforms.size(); ++i) {
            if (game.platforms[i]->isDisabled()) continue;
            if (samePosition(game.platforms[i])) continue;
            //std::cout << "i = " << i << "\n";
            if (!platCheck(leftQuad, rightQuad, true, game.platforms[i])) return false;
        }
    }
    else { // quads13
        switch (orientation) {
        case dir_up:
        case dir_left:
            leftQuad = TILE_WIDTH * leftTiles;
            rightQuad = TILE_WIDTH * rightTiles;
            break;
        case dir_down:
        case dir_right:
            leftQuad = TILE_WIDTH * rightTiles;
            rightQuad = TILE_WIDTH * leftTiles;
            break;
        }
        //std::cout << "rightQuad " << rightQuad << "\n";
        for (size_t i = 0; i<game.platforms.size(); ++i) {
            if (game.platforms[i]->isDisabled()) continue;
            if (samePosition(game.platforms[i])) continue;
            //std::cout << "i = " << i << "\n";
            if (!platCheck(leftQuad, rightQuad, false, game.platforms[i])) return false;
        }
    }
    return true;
}

bool Platform::platCheck(int leftQuad, int rightQuad, bool quads24, Platform* other) { // quads24 ? quads 2-4 : quads 1-3
    Point centerPivot(x, y);
    int centerx1 = other->x - TILE_WIDTH / 2;
    int centerx2 = other->x + TILE_WIDTH / 2;
    int centery1 = other->y - TILE_WIDTH / 2;
    int centery2 = other->y + TILE_WIDTH / 2;

    switch (other->orientation) {
    case dir_up: {
        for (int i = 0; i < other->leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1 - TILE_WIDTH * i, centery1,
                centerx1 - TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other->rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx2 + TILE_WIDTH * i, centery1,
                centerx2 + TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    case dir_down: {
        for (int i = 0; i < other->leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx2 + TILE_WIDTH * i, centery1,
                centerx2 + TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other->rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1 - TILE_WIDTH * i, centery1,
                centerx1 - TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    case dir_right: {
        for (int i = 0; i < other->leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery1 - TILE_WIDTH * i,
                centerx2, centery1 - TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other->rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery2 + TILE_WIDTH * i,
                centerx2, centery2 + TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    case dir_left: {
        for (int i = 0; i < other->leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery2 + TILE_WIDTH * i,
                centerx2, centery2 + TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other->rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery1 - TILE_WIDTH * i,
                centerx2, centery1 - TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    }
    return true;
}

bool Platform::twoPointsTwoDistances(Point center, int p1x, int p1y, int p2x, int p2y, int lQuad, int rQuad, bool quads24) {
    Point one(p1x, p1y);
    Point two(p2x, p2y);
    if (quads24) {
        if (p1x < center.x && p1y < center.y && one.distance(center) < lQuad) {
            //std::cout << "a p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad2
        }
        if (p1x > center.x && p1y > center.y && one.distance(center) < rQuad) {
            //std::cout << "b p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad4
        }
        if (p2x < center.x && p2y < center.y && two.distance(center) < lQuad) {
            //std::cout << "c p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad2
        }
        if (p2x > center.x && p2y > center.y && two.distance(center) < rQuad) {
            //std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad4
        }
    }
    else {
        if (p1x < center.x && p1y > center.y && one.distance(center) < lQuad) {
            //std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad3
        }
        if (p1x > center.x && p1y < center.y && one.distance(center) < rQuad) {
            //std::cout << "e p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad1
        }
        if (p2x < center.x && p2y > center.y && two.distance(center) < lQuad) {
            //std::cout << "f p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad3
        }
        if (p2x > center.x && p2y < center.y && two.distance(center) < rQuad) {
            //std::cout << "g p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            //std::cout << "First condition " << (p2x > center.x) << "\n";
            //std::cout << "Second condition " << (p2y < center.y) << "\n";
            //std::cout << "Distance from center " << two.distance(center) << "\n";
            //std::cout << "Spin radius " << rQuad << "\n";
            return false; // quad1
        }
    }
    /*
    if (quads24) {
    if (p1x < center.x && p1y > center.y && one.distance(center) < lQuad) {
    std::cout << "a p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad2
    }
    if (p1x > center.x && p1y < center.y && one.distance(center) < rQuad) {
    std::cout << "b p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad4
    }
    if (p2x < center.x && p2y > center.y && two.distance(center) < lQuad) {
    std::cout << "c p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad2
    }
    if (p2x > center.x && p2y < center.y && two.distance(center) < rQuad) {
    std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad4
    }
    } else {
    if (p1x < center.x && p1y < center.y && one.distance(center) < lQuad) {
    std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad3
    }
    if (p1x > center.x && p1y > center.y && one.distance(center) < rQuad) {
    std::cout << "e p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad1
    }
    if (p2x < center.x && p2y < center.y && two.distance(center) < lQuad) {
    std::cout << "f p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad3
    }
    if (p2x > center.x && p2y > center.y && two.distance(center) < rQuad) {
    std::cout << "g p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad1
    }
    }*/
    return true;
}


bool Platform::samePosition(Platform* other) {
    return cx == other->cx && cy == other->cy;
}


void Platform::temporarilyDisableLaserSources() {
    for (size_t i = 0, n = laserSources.size(); i < n; ++i) {
        laserSources[i]->temporarilyTurnOffLaser();
    }
}

void Platform::repositionAttachedObjects() {
    for (size_t i = 0, n = laserSources.size(); i < n; ++i){
        laserSources[i]->updateActualPosition();
    }

    for (size_t i = 0, n = laserTargets.size(); i < n; ++i) {
        laserTargets[i]->updateActualPosition();
    }
}

void Platform::addLaserSource(LaserSource* laserSource) {
    this->laserSources.push_back(laserSource);
}

void Platform::addLaserTarget(LaserTarget* laserTarget) {
    this->laserTargets.push_back(laserTarget);
}