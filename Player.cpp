#include <iostream>
#include "globals.h"
#include "Player.h"
#include "gamemath.h"
#include "GameGlobals.h"
#include "Textures.h"

Player::Player(){}

Player::Player(int cx, int cy) {
    isActive = true;
    x = 0; y = 0;
    gridToActual(cx, cy, &x, &y);
    vx = 0; vy = 0;
    speed = 5;
    jumpSpeed = 9;
    gravity = 0.6f;
    gravityX = 0;
    gravityY = 0;
    pwidth = 35;
    pheight = 40;
    facingRight = true;
    isRotating = false;
    setOrientation(dir_up);

    sprite.setTexture(textures->player);
    shape = sf::RectangleShape();
    shape.setFillColor(sf::Color::Blue);

    currentPlatform = &nullPlatform;
    frozenPlatform = &nullPlatform;
}

bool Player::canRotate(bool right) {
    if (isControlsDisabled()) return false;
    if (currentPlatform->isNull) return false;
    if (!currentPlatform->rotatable) return false;
    if (currentPlatform->isRotationDisabled) return false;
    return orientation == currentPlatform->orientation;
}

bool Player::rotatesIntoPlatform() {
    int _x = x;
    int _y = y;
    rotateAboutPivotActual(orientation, targetOrientation, currentPlatform->cx, currentPlatform->cy, &_x, &_y);
    
    int halfWidth = pwidth / 2;
    int halfHeight = pheight / 2;

    int _x1 = _x;
    int _x2 = _x;
    int _y1 = _y;
    int _y2 = _y;
    switch (targetOrientation) {
    case dir_up:
    case dir_down:
        _x1 -= halfWidth;
        _x2 += halfWidth;
        _y1 -= halfHeight;
        _y2 += halfHeight;
        break;
    case dir_left:
    case dir_right:
        _x1 -= halfHeight;
        _x2 += halfHeight;
        _y1 -= halfWidth;
        _y2 += halfWidth;
    }

    for (size_t i = 0, n = game.platforms.size(); i < n; ++i) {
        Platform* platform = game.platforms[i];
        if (currentPlatform->samePosition(platform)) continue; // Exception: can collide with the current platform.
        if (platform->isDisabled()) continue;
        if (collidesWithTargetOrientation(_x1, _y1, _x2, _y2, platform)) return true;
    }
    return false;
}

bool Player::collidesWithTargetOrientation(float _x1, float _y1, float _x2, float _y2, Platform* plat) {
    return (_x2 > plat->x1 && _x1 < plat->x2 && _y2 > plat->y1 && _y1 < plat->y2);
}

void Player::finishRotating(bool success) {
    isRotating = false;
    this->angle = orientationToAngle(currentPlatform->orientation);

    if (success) {
        rotateAboutPivotActual(orientation, targetOrientation, currentPlatform->cx, currentPlatform->cy, &x, &y);
        setOrientation(targetOrientation);
        updateBoundaries();
    }
}

void Player::rotateTo(int newOrientation) {
    this->isRotating = true;
    this->rotatingBaseDX = this->x - currentPlatform->x;
    this->rotatingBaseDY = this->y - currentPlatform->y;
    rotateVector(&rotatingBaseDX, &rotatingBaseDY, -orientationToAngle(orientation));

    vx = 0; vy = 0;
    targetOrientation = newOrientation;
    currentPlatform->rotateTo(targetOrientation);
}

void Player::getSpriteCoordinates(float* sx, float* sy) {
    float dx = this->rotatingBaseDX;
    float dy = this->rotatingBaseDY;
    rotateVector(&dx, &dy, angle);
    *sx = dx + currentPlatform->x;
    *sy = dy + currentPlatform->y;
}

void Player::draw() {
    if (!isActive) return;

    float w = pheight * 1 / 2;
    float tlx, tly, blx, bly, brx, bry;

    if (isRotating) {
        float platX = currentPlatform->x;
        float platY = currentPlatform->y;
        float dx = this->rotatingBaseDX;
        float dy = this->rotatingBaseDY;

        generateRotatedCorners(dx-w, dy-w, dx+w, dy+w, &tlx, &tly, &blx, &bly, &brx, &bry, currentPlatform->angle);
        drawSprite(&sprite, platX + tlx, platY + tly, platX + blx, platY + bly, platX + brx, platY + bry, !facingRight);
    }
    else {
        generateRotatedCorners(-w, -w, w, w, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
        drawSprite(&sprite, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry, !facingRight);
    }
}

void Player::updateBoundaries() {
    x1 = x;
    x2 = x;
    y1 = y;
    y2 = y;
    switch (orientation) {
    case dir_up:
    case dir_down:
        x1 -= pwidth / 2;
        x2 += pwidth / 2;
        y1 -= pheight / 2;
        y2 += pheight / 2;
        break;
    case dir_left:
    case dir_right:
        x1 -= pheight / 2;
        x2 += pheight / 2;
        y1 -= pwidth / 2;
        y2 += pwidth / 2;
        break;
    default:
        std::cout << "Nu\n";
    }

}

void Player::update(Keyboard k) {
    if (!isActive) return;

    if (isRotating) {
        this->angle = currentPlatform->angle;
        if (currentPlatform->isRotating) return;

        // platform has stopped rotating.
        this->finishRotating(currentPlatform->isRotationSuccessful);
        game.finishRotatingTrigger();
    }

    if (!currentPlatform->isNull && currentPlatform->isDisabled()) {
        currentPlatform = &nullPlatform;
    }

    updateBoundaries();
    if (isTouchingLaser()) {
        die();
    }

    if (game.grid.isOutOfBounds(x, y)) {
        die();
    }


    // Fall off the sides
    switch (this->orientation) {
    case dir_up:
    case dir_down:
        if (!currentPlatform->isNull)
            if (x1 >= currentPlatform->x2 || x2 <= currentPlatform->x1) currentPlatform = &nullPlatform;
        break;
    case dir_left:
    case dir_right:
        if (!currentPlatform->isNull)
            if (y1 >= currentPlatform->y2 || y2 <= currentPlatform->y1) currentPlatform = &nullPlatform;
        break;
    default:
        std::cout << "Wat\n";
    }

    // Do not fly off
    if (!currentPlatform->isNull) {
        switch (this->orientation) {
        case dir_up: if (y2 != currentPlatform->y1) currentPlatform = &nullPlatform; break;
        case dir_down: if (y1 != currentPlatform->y2) currentPlatform = &nullPlatform; break;
        case dir_left: if (x2 != currentPlatform->x1) currentPlatform = &nullPlatform; break;
        case dir_right: if (x1 != currentPlatform->x2) currentPlatform = &nullPlatform; break;
        default: std::cout << "Wut\n";
        }
    }

    if (currentPlatform->isNull) {
        vx += gravityX;
        vy += gravityY;
    }

    // Speed Control + What do left and right do
    switch (this->orientation) {
    case dir_up:
        vx = 0;
        if (k.left) vx -= speed;
        if (k.right) vx += speed;
        break;
    case dir_down:
        vx = 0;
        if (k.left) vx += speed;
        if (k.right) vx -= speed;
        break;
    case dir_left:
        vy = 0;
        if (k.left) vy += speed;
        if (k.right) vy -= speed;
        break;
    case dir_right:
        vy = 0;
        if (k.left) vy -= speed;
        if (k.right) vy += speed;
        break;
    default:
        std::cout << "Whyyy\n";
    }

    if (k.left && !k.right) facingRight = false;
    if (k.right && !k.left) facingRight = true;

    // TODO: Possibly Add code to force players out of the nearest platform, just in case somehow the player gets stuck in a platform.

    // Move Player
    tryMoveInDirection(vx, vy);
}

void Player::tryMoveInDirection(int dx, int dy) {
    if (dx == 0 && dy == 0) return;

    int moveX;
    int moveY;
    int minMoveDistance = -1;
    Platform* targetPlatform = nullptr;
    int hitPlatformDirection = -1;

    // Find the closest colliding platform (moveDistance is used to judge the closeness)
    for (size_t i = 0; i<game.platforms.size(); ++i) {
        Platform* platform = game.platforms[i];
        if (platform->isDisabled()) continue;

        int mx, my = 0;
        int hitDirection = -1;
        int moveDistance = getPlatformMoveDistance(x, y, dx, dy, platform, &mx, &my, &hitDirection);
        if (moveDistance == -1) continue;

        if (minMoveDistance == -1 || moveDistance < minMoveDistance) {
            minMoveDistance = moveDistance;
            targetPlatform = platform;
            hitPlatformDirection = hitDirection;
            moveX = mx;
            moveY = my;
        }
    }

    // Move the player.
    if (minMoveDistance == -1) {
        // No blocking platform
        x += dx;
        y += dy;
    } else {
        // blocked
        x += moveX;
        y += moveY;

        if (hitPlatformDirection == this->orientation) setCurrentPlatform(targetPlatform);
        switch (hitPlatformDirection) {
        case dir_up:
        case dir_down:
            vy = 0;
            dx -= moveX;
            dy = 0;
            break;
        case dir_left:
        case dir_right:
            vx = 0;
            dx = 0;
            dy -= moveY;
            break;
        }

        // Continue movement with the remaining dx,dy.
        tryMoveInDirection(dx, dy);
    }
}

// Get the maximum distance the player can go before colliding with the platform.
// Returns: moveDistance, moveX, moveY, hitDirection.
int Player::getPlatformMoveDistance(int fromX, int fromY, int dx, int dy, Platform* plat, int* moveX, int* moveY, int* hitDirection) {
    if (!(x2+dx > plat->x1 && x1+dx < plat->x2 && y2+dy > plat->y1 && y1+dy < plat->y2)) {
        return -1;
    }

    int xWidth, yWidth;
    switch (orientation) {
    case dir_up:
    case dir_down:
        xWidth = pwidth / 2;
        yWidth = pheight / 2;
        break;
    case dir_left:
    case dir_right:
        xWidth = pheight / 2;
        yWidth = pwidth / 2;
        break;
    }

    int signX = (dx > 0) ? 1 : -1;
    int signY = (dy > 0) ? 1 : -1;

    int moveDistance = std::numeric_limits<int>::min();
    if (dx > 0) {
        int maxX = plat->x1;
        if (dx > maxX - fromX - xWidth) {
            int mx = maxX - fromX - xWidth;
            int my = mx * dy / dx;

            moveDistance = mx / signX + my / signY;
            *moveY = my;
            *moveX = mx;
            *hitDirection = dir_left;
        }
    } else if (dx < 0) {
        int minX = plat->x2;
        if (dx < minX - fromX + xWidth) {
            int mx = minX - fromX + xWidth;
            int my = mx * dy / dx;

            moveDistance = mx / signX + my / signY;
            *moveY = my;
            *moveX = mx;
            *hitDirection = dir_right;
        }
    }

    if (dy > 0) {
        int maxY = plat->y1;
        if (dy > maxY - fromY - yWidth) {
            int my = maxY - fromY - yWidth;
            int mx = my * dx / dy;

            int newMoveDistance = mx / signX + my / signY;
            if (newMoveDistance > moveDistance) {
                *moveY = my;
                *moveX = mx;
                moveDistance = newMoveDistance;
                *hitDirection = dir_up;
            }
        }
    } else if (dy < 0) {
        int minY = plat->y2;
        if (dy < minY - fromY + yWidth) {
            int my = minY - fromY + yWidth;
            int mx = my * dx / dy;

            int newMoveDistance = mx / signX + my / signY;
            if (newMoveDistance > moveDistance) {
                *moveY = my;
                *moveX = mx;
                moveDistance = newMoveDistance;
                *hitDirection = dir_down;
            }
        }
    }

    return moveDistance;
}


void Player::getGridCoordinates(int* gridX, int* gridY) {
    actualToGrid(x, y, gridX, gridY);
}

void Player::jump() {
    if (isControlsDisabled()) return;

    if (!currentPlatform->isNull && !currentPlatform->isDisabled()) {
        currentPlatform = &nullPlatform; // isNull
        switch (orientation) {
        case dir_up: vy = (-1) * jumpSpeed; break;
        case dir_down: vy = jumpSpeed; break;
        case dir_left: vx = (-1) * jumpSpeed; break;
        case dir_right: vx = jumpSpeed; break;
        }
    }
}

void Player::setOrientation(int orientation) {
    this->orientation = orientation;

    switch (orientation) {
    case dir_up: {
        gravityX = 0;
        gravityY = gravity;
        break;
    }
    case dir_down: {
        gravityX = 0;
        gravityY = (-1) * gravity;
        break;
    }
    case dir_left: {
        gravityX = gravity;
        gravityY = 0;
        break;
    }
    case dir_right: {
        gravityX = (-1) * gravity;
        gravityY = 0;
        break;
    }
    }

    this->angle = orientationToAngle(orientation);
}

// Currently not in use.
void Player::collision(Platform* plat) {
    if (x2 > plat->x1 && x1 < plat->x2 && y2 > plat->y1 && y1 < plat->y2) {
        int touchSide = -1;
        int closestDist = TILE_WIDTH * 2;
        if (x2 - plat->x1 < closestDist) {
            touchSide = dir_left;
            closestDist = x2 - plat->x1;
        }
        if (plat->x2 - x1 < closestDist) {
            touchSide = dir_right;
            closestDist = plat->x2 - x1;
        }
        if (y2 - plat->y1 < closestDist) {
            touchSide = dir_up;
            closestDist = y2 - plat->y1;
        }
        if (plat->y2 - y1 < closestDist) {
            touchSide = dir_down;
            closestDist = plat->y2 - y1;
        }

        switch (touchSide) {
        case dir_left: {
            x -= closestDist;
            if (vx > 0) vx = 0;
            if (this->orientation == dir_left) setCurrentPlatform(plat);
            break;
        }
        case dir_right: {
            x += closestDist;
            if (vx < 0) vx = 0;
            if (this->orientation == dir_right) setCurrentPlatform(plat);
            break;
        }
        case dir_up: {
            y -= closestDist;
            if (vy > 0) vy = 0;
            if (this->orientation == dir_up) setCurrentPlatform(plat);
            break;
        }
        case dir_down: {
            y += closestDist;
            if (vy < 0) vy = 0;
            if (this->orientation == dir_down) setCurrentPlatform(plat);
            break;
        }
        default: {
            std::cout << "Why\n";
        }
        }
    }
}

void Player::snapToCurrentPlatformHeight() {
    switch (this->orientation) {
    case dir_up: this->y = currentPlatform->y1 - pheight / 2; break;
    case dir_down: this->y = currentPlatform->y2 + pheight / 2; break;
    case dir_left: this->x = currentPlatform->x1 - pheight / 2; break;
    case dir_right: this->x = currentPlatform->x2 + pheight / 2; break;
    }
}

void Player::setCurrentPlatform(Platform* plat) {
    this->currentPlatform = plat;
    this->snapToCurrentPlatformHeight();
}

bool Player::isControlsDisabled() {
    return (!isActive || isRotating);
}

void Player::die() {
    this->isActive = false;
}

bool Player::isTouchingLaser() {
    for (size_t i = 0, n = game.lasers.size(); i < n; ++i){
        Laser* l = game.lasers[i];
        if (rectsIntersect(x-5, y-5, x+5, y+5, l->x1, l->y1, l->x2, l->y2)) return true;
    }
    return false;
}

bool Player::isWithinClickHitbox(int sx, int sy) {
    updateBoundaries();
    return isWithinRectSafe(sx, sy, x1, y1, x2, y2);
}