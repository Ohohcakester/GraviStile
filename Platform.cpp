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
#include "Player.h"
#include "Grid.h"

Platform::Platform() {
    this->isNull = true;
}

Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, bool rotatable, int orientation, bool isDisabled, bool isRotationDisabled) :
    cx(cx), cy(cy), leftTiles(leftTiles), rightTiles(rightTiles), rotatable(rotatable), orientation(orientation), isRotationDisabled(isRotationDisabled)
{
    if (isDisabled) this->disabledStatus = platformStatus_disabled;
    else this->disabledStatus = platformStatus_enabled;

    spinConnection = NULL;
    this->isNull = false;
    this->isRevertingToPreviousRotation = true;
    gridToActual(cx, cy, &this->x, &this->y);

    setOrientation(orientation);
    finishRotation();

    sprite.setTexture(textures->pivot);
    shape = sf::RectangleShape();
    shape.setFillColor(textures->platformColor);
    extraLineShape = sf::RectangleShape();
    extraLineShape.setFillColor(textures->platformSurfaceColor);
    pivotShape = sf::CircleShape(game.zoom*TILE_WIDTH / 3);
    pivotShape.setFillColor(sf::Color::Magenta);
    isUsingDisabledGraphic = false;
}

void Platform::updateUsingDisabledGraphic() {
    bool isDisabled = this->isDisabled();
    if (isDisabled != isUsingDisabledGraphic) {
        isUsingDisabledGraphic = isDisabled;
        if (isDisabled) {
            shape.setFillColor(textures->platformDisabledColor);
            extraLineShape.setFillColor(textures->platformDisabledSurfaceColor);
        } else {
            shape.setFillColor(textures->platformColor);
            extraLineShape.setFillColor(textures->platformSurfaceColor);
        }
    }
}

void Platform::draw() {
    //isUsingDisabledGraphic = !isDisabled(); // DEBUG CODE
    updateUsingDisabledGraphic();

    /*std::vector<Platform*>* ot = &game.platforms;  // DEBUG CODE
    for (size_t i = 0, n = ot->size(); i < n; ++i) {  // DEBUG CODE
        Platform* p = (*ot)[i];
        if (this->samePosition(p)) continue;
        if (this->collidesWith(p)) shape.setFillColor(textures->laserColor);
    }*/


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

        if (!this->isRotationDisabled && !this->isDisabled()) {
            float radius = game.zoom*TILE_WIDTH / 3;
            generateRotatedCorners(-radius, -radius, radius, radius, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
            drawSprite(&sprite, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry);
        }
    }
}

void Platform::update(Keyboard k) {
    if (isRotating) {
        if (!isRevertingToPreviousRotation && isObstructedWhileRotating()) {
            cancelRotation();
            return;
        }

        float diff = clampedAngularDifference(angle, targetAngle);
        if (diff < 0) {
            angle -= ROTATE_SPEED;
            diff = clampedAngularDifference(angle, targetAngle);
            if (diff >= 0) {
                onReach();
            }
        } else {
            angle += ROTATE_SPEED;
            diff = clampedAngularDifference(angle, targetAngle);
            if (diff < 0) {
                onReach();
            }
        }
    }
}

bool Platform::isObstructedWhileRotating() {
    if (this->isDisabled()) return false;

    std::vector<Platform*>* platforms = &game.platforms;
    for (size_t i = 0, n = platforms->size(); i < n; ++i) {
        Platform* p = (*platforms)[i];
        if (this->samePosition(p)) continue;
        if (p->isDisabled()) continue;
        if (this->collidesWith(p)) return true;
    }
    return false;
}

void Platform::finishRotation() {
    angle = targetAngle;
    isRotating = false;
    isRotationSuccessful = !isRevertingToPreviousRotation;
    isRevertingToPreviousRotation = false;
    this->repositionAttachedObjects();
}

void Platform::onReach() {
    if (!isRevertingToPreviousRotation && game.player.rotatesIntoPlatform()) {
        cancelRotation();
        return;
    }

    if (spinConnection != NULL) {
        spinConnection->finishRotation();
    } else {
        this->finishRotation();
    }
}

void Platform::cancelRotation() {
    if (spinConnection == NULL) {
        revertToPreviousOrientation();
    }
    else {
        spinConnection->revertToPreviousOrientation();
    }
}

bool Platform::isDisabled() {
    return disabledStatus != platformStatus_enabled;
}

bool Platform::isBlockedFromUndisabling(Grid* grid, Player* player) {
    // Collides with another platform.
    switch (orientation) {
    case dir_up: {
        int minX = cx - leftTiles;
        int maxX = cx + rightTiles;
        int y = cy;
        for (int x = minX; x <= maxX; ++x) if (grid->isBlocked(x, y)) return true;
        break;
    }
    case dir_right: {
        int minY = cy - leftTiles;
        int maxY = cy + rightTiles;
        int x = cx;
        for (int y = minY; y <= maxY; ++y) if (grid->isBlocked(x, y)) return true;
        break;
    }
    case dir_down: {
        int minX = cx - rightTiles;
        int maxX = cx + leftTiles;
        int y = cy;
        for (int x = minX; x <= maxX; ++x) if (grid->isBlocked(x, y)) return true;
        break;
    }
    case dir_left: {
        int minY = cy - rightTiles;
        int maxY = cy + leftTiles;
        int x = cx;
        for (int y = minY; y <= maxY; ++y) if (grid->isBlocked(x, y)) return true;
        break;
    }
    }

    // Collides with player
    if (player->x2 > x1 && player->x1 < x2 && player->y2 > y1 && player->y1 < y2) return true;

    return false;
}

bool Platform::tryUnblockFromDisabled(Grid* grid, Player* player) {
    if (disabledStatus == platformStatus_waitingForEnable && !isBlockedFromUndisabling(grid, player)) {
        disabledStatus = platformStatus_enabled;
        return true;
    }
    return false;
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
        saveCurrentOrientation();
        setOrientation(newOrientation);
    }
    else {
        spinConnection->rotateBy(orientation, newOrientation);
    }
}


void Platform::saveCurrentOrientation() {
    previousOrientation = orientation;
}

void Platform::revertToPreviousOrientation() {
    isRevertingToPreviousRotation = true;
    setOrientation(previousOrientation);
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

bool Platform::collidesWith(Platform* o) {
    return this->oneSidedCollidesWith(o) && o->oneSidedCollidesWith(this);
}

bool Platform::oneSidedCollidesWith(Platform* o) {
    float left = -TILE_WIDTH * (leftTiles + 0.25); // don't add 0.5
    float right = TILE_WIDTH * (rightTiles + 0.25);

    float _x1b = -TILE_WIDTH * (o->leftTiles + 0.25); // don't add 0.5
    float _x2b = TILE_WIDTH * (o->rightTiles + 0.25);

    float minY = -TILE_WIDTH * 0.4f;
    float maxY = TILE_WIDTH * 0.4f;

    float tlx2, tly2, blx2, bly2, brx2, bry2, trx2, try2;
    generateRotatedCorners(_x1b, minY, _x2b, maxY, &tlx2, &tly2, &blx2, &bly2, &brx2, &bry2, &trx2, &try2, o->angle - angle);

    float dx = o->x - x;
    float dy = o->y - y;
    rotateVector(&dx, &dy, -angle);

    tlx2 += dx;
    blx2 += dx;
    trx2 += dx;
    brx2 += dx;
    tly2 += dy;
    bly2 += dy;
    try2 += dy;
    bry2 += dy;

    /*{ // Debugging code
        sf::RectangleShape s;
        s.setFillColor(textures->laserTargetColor);
        drawRectangle(&s, left, minY, left, maxY, right, maxY);
        sf::RectangleShape s2;
        s2.setFillColor(textures->laserSourceColor);
        drawRectangle(&s2, tlx2, tly2, blx2, bly2, brx2, bry2);
    }*/

    if (tlx2 > right && blx2 > right && trx2 > right && brx2 > right) return false;
    if (tlx2 < left && blx2 < left && trx2 < left && brx2 < left) return false;
    if (tly2 > maxY && bly2 > maxY && try2 > maxY && bry2 > maxY) return false;
    if (tly2 < minY && bly2 < minY && try2 < minY && bry2 < minY) return false;
    return true;
}