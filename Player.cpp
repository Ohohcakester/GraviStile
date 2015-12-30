#include <iostream>
#include "globals.h"
#include "Player.h"
#include "gamemath.h"

Player::Player(){}

Player::Player(int cx, int cy) {
    x = 0; y = 0;
    gridToActual(cx, cy, &x, &y);
    vx = 0; vy = 0;
    speed = 5;
    jumpSpeed = 11;
    gravity = 0.7f;
    gravityX = 0;
    gravityY = 0;
    pwidth = 30;
    pheight = 50;
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
    if (isRotating) return false;
    if (currentPlatform->isNull) return false;
    if (!currentPlatform->rotatable) return false;
    if (!currentPlatform->sweep(right)) return false;
    return orientation == currentPlatform->orientation;
}

void Player::rotateTo(int newOrientation) {
    this->isRotating = true;
    this->rotatingBaseDX = this->x - currentPlatform->x;
    this->rotatingBaseDY = this->y - currentPlatform->y;
    rotateVector(&rotatingBaseDX, &rotatingBaseDY, -orientationToAngle(orientation));

    vx = 0; vy = 0;
    rotateAboutPivotActual(orientation, newOrientation, currentPlatform->cx, currentPlatform->cy, &x, &y);
    setOrientation(newOrientation);
    currentPlatform->setOrientation(newOrientation);
    updateBoundaries();
}

void Player::getSpriteCoordinates(float* sx, float* sy) {
    float dx = this->rotatingBaseDX;
    float dy = this->rotatingBaseDY;
    rotateVector(&dx, &dy, angle);
    *sx = dx + currentPlatform->x;
    *sy = dy + currentPlatform->y;
}

void Player::draw() {
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
    if (isRotating) {
        this->angle = currentPlatform->angle;
        if (currentPlatform->isRotating) return;

        // platform has stopped rotating.
        this->angle = orientationToAngle(currentPlatform->orientation);
        isRotating = false;
    }

    updateBoundaries();
    // Fall off the sides
    switch (this->orientation) {
    case dir_up:
    case dir_down:
        if (!currentPlatform->isNull)
            if (x1 >= currentPlatform->x2 || x2 <= currentPlatform->x1) currentPlatform = &nullPlatform;
        vx = 0; // also sneak in speed control
        break;
    case dir_left:
    case dir_right:
        if (!currentPlatform->isNull)
            if (y1 >= currentPlatform->y2 || y2 <= currentPlatform->y1) currentPlatform = &nullPlatform;
        vy = 0;
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

    // What do left and right do
    switch (this->orientation) {
    case dir_up:
        if (k.left) vx -= speed;
        if (k.right) vx += speed;
        break;
    case dir_down:
        if (k.left) vx += speed;
        if (k.right) vx -= speed;
        break;
    case dir_left:
        if (k.left) vy += speed;
        if (k.right) vy -= speed;
        break;
    case dir_right:
        if (k.left) vy -= speed;
        if (k.right) vy += speed;
        break;
    default:
        std::cout << "Whyyy\n";
    }
    if (k.left) facingRight = false;
    if (k.right) facingRight = true;

    x += vx;
    y += vy;

    // std::cout << "x = " << x << " y = " << y << "\n";

    for (int i = 0; i<game.platforms.size(); ++i) {
        collision(&game.platforms[i]);
    }
    /*for (std::vector<Platform>::iterator it = game.platforms.begin(); it != game.platforms.end(); ++it) {
    collision(it);
    }*/
}

void Player::getGridCoordinates(int* gridX, int* gridY) {
    actualToGrid(x + TILE_WIDTH / 2, y + TILE_WIDTH / 2, gridX, gridY);
}

void Player::jump() {
    if (isRotating) return;
    if (!currentPlatform->isNull) {
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
            if (this->orientation == dir_left) currentPlatform = plat;
            break;
        }
        case dir_right: {
            x += closestDist;
            if (vx < 0) vx = 0;
            if (this->orientation == dir_right) currentPlatform = plat;
            break;
        }
        case dir_up: {
            y -= closestDist;
            if (vy > 0) vy = 0;
            if (this->orientation == dir_up) currentPlatform = plat;
            break;
        }
        case dir_down: {
            y += closestDist;
            if (vy < 0) vy = 0;
            if (this->orientation == dir_down) currentPlatform = plat;
            break;
        }
        default: {
            std::cout << "Why\n";
        }
        }
    }
}