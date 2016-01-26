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
#include "Sfx.h"

Platform::Platform() {
    this->isNull = true;
}

Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, bool rotatable, int orientation, bool isDisabled, bool isRotationDisabled) :
    cx(cx), cy(cy), leftTiles(leftTiles), rightTiles(rightTiles), rotatable(rotatable), orientation(orientation), isRotationDisabled(isRotationDisabled)
{
    if (isDisabled) this->disabledStatus = platformStatus_disabled;
    else this->disabledStatus = platformStatus_enabled;

    spinConnection = nullptr;
    this->isNull = false;
    this->isRevertingToPreviousRotation = true;
    gridToActual(cx, cy, &this->x, &this->y);

    setOrientation(orientation);
    finishRotation();

    sprite.setTexture(global::textures->pivot);
    shape = sf::RectangleShape();
    shape.setFillColor(global::textures->platformColor);
    extraLineShape = sf::RectangleShape();
    extraLineShape.setFillColor(global::textures->platformSurfaceColor);
    pivotShape = sf::CircleShape(global::game.zoom*global::TILE_WIDTH / 3);
    pivotShape.setFillColor(sf::Color::Magenta);
    isUsingDisabledGraphic = false;
}

void Platform::updateUsingDisabledGraphic() {
    bool isDisabled = this->isDisabled();
    if (isDisabled != isUsingDisabledGraphic) {
        Textures* textures = global::textures;

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
    
    float _x1 = -global::TILE_WIDTH * (leftTiles + 0.5);
    float _x2 = global::TILE_WIDTH * (rightTiles + 0.5);
    float _y1 = -global::TILE_WIDTH / 2;
    float _y2 = global::TILE_WIDTH / 2;

    float tlx, tly, blx, bly, brx, bry;
    generateRotatedCorners(_x1, _y1, _x2, _y2, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
    drawRectangle(&shape, x+tlx, y+tly, x+blx, y+bly, x+brx, y+bry);

    if (this->rotatable) {
        int fraction = 5;
        float _ey2 = _y1 + (_y2 - _y1) / fraction;
        generateRotatedCorners(_x1, _y1, _x2, _ey2, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
        drawRectangle(&extraLineShape, x+tlx, y+tly, x+blx, y+bly, x+brx, y+bry);

        if (!this->isRotationDisabled && !this->isDisabled()) {
            float radius = global::game.zoom*global::TILE_WIDTH / 3;
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
            angle -= global::ROTATE_SPEED;
            diff = clampedAngularDifference(angle, targetAngle);
            if (diff >= 0) {
                onReach();
            }
        } else {
            angle += global::ROTATE_SPEED;
            diff = clampedAngularDifference(angle, targetAngle);
            if (diff < 0) {
                onReach();
            }
        }
    }
}

bool Platform::isObstructedWhileRotating() {
    if (this->isDisabled()) return false;

    std::vector<Platform*>* platforms = &global::game.platforms;
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
    if (!isRevertingToPreviousRotation && global::game.player.rotatesIntoPlatform()) {
        cancelRotation();
        return;
    }

    if (spinConnection != nullptr) {
        spinConnection->finishRotation();
    } else {
        this->finishRotation();
    }
}

void Platform::cancelRotation() {
    if (spinConnection == nullptr) {
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
        playEnableAnimation();
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
        playDisableAnimation();
        break;
    case platformStatus_enabled:
        disabledStatus = platformStatus_disabled;
        playDisableAnimation();
        break;
    }
}

void Platform::toggleRotationDisabledStatus() {
    isRotationDisabled = !isRotationDisabled;
}

void Platform::rotateTo(int newOrientation) {
    if (spinConnection == nullptr) {
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
    int tileWidth = global::TILE_WIDTH;
    this->orientation = orientation;

    this->x1 = x - tileWidth / 2;
    this->x2 = x + tileWidth / 2;
    this->y1 = y - tileWidth / 2;
    this->y2 = y + tileWidth / 2;

    if (orientation == dir_up) {
        this->x1 -= tileWidth * leftTiles;
        this->x2 += tileWidth * rightTiles;
    }

    if (orientation == dir_right) {
        this->y1 -= tileWidth * leftTiles;
        this->y2 += tileWidth * rightTiles;
    }

    if (orientation == dir_down) {
        this->x1 -= tileWidth * rightTiles;
        this->x2 += tileWidth * leftTiles;
    }

    if (orientation == dir_left) {
        this->y1 -= tileWidth * rightTiles;
        this->y2 += tileWidth * leftTiles;
    }

    this->targetAngle = orientationToAngle(orientation);
    this->isRotating = true;
    this->temporarilyDisableLaserSources();
}

bool Platform::isUnderDoor(int doorX, int doorY, int doorOrientation) {
    switch (doorOrientation) {
    case dir_up:
        doorY++; break;
    case dir_right:
        doorX--; break;
    case dir_down:
        doorY--; break;
    case dir_left:
        doorX++; break;
    }

    switch (orientation) {
    case dir_up:
        return (cy == doorY) && (cx - leftTiles <= doorX && doorX <= cx + rightTiles);
    case dir_down:
        return (cy == doorY) && (cx - rightTiles <= doorX && doorX <= cx + leftTiles);
    case dir_left:
        return (cx == doorX) && (cy - rightTiles <= doorY && doorY <= cy + leftTiles);
    case dir_right:
        return (cx == doorX) && (cy - leftTiles <= doorY && doorY <= cy + rightTiles);
    }
    return false;
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
    float left = -global::TILE_WIDTH * (leftTiles + 0.25); // don't add 0.5
    float right = global::TILE_WIDTH * (rightTiles + 0.25);

    float _x1b = -global::TILE_WIDTH * (o->leftTiles + 0.25); // don't add 0.5
    float _x2b = global::TILE_WIDTH * (o->rightTiles + 0.25);

    float minY = -global::TILE_WIDTH * 0.4f;
    float maxY = global::TILE_WIDTH * 0.4f;

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


bool Platform::isWithinClickHitbox(int x, int y) {
    return isWithinRect(x, y, x1, y1, x2, y2);
}

void Platform::playDisableAnimation() {
    global::game.spawnNewSfx(new sfx::PlatformChange(x, y, true));
}

void Platform::playEnableAnimation() {
    global::game.spawnNewSfx(new sfx::PlatformChange(x, y, false));

}