#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "gameobjects.h"
#include "keyboard.h"
#include "globals.h"


void IGameObject::drawCircle(sf::CircleShape* shape, float px, float py) {
    if (!freeze) {
        game.camera.toRel(&px, &py);

        shape->setOrigin(-px+shape->getRadius(),-py+shape->getRadius());
    }
    window.draw(*shape);
}

void IGameObject::drawRectangle(sf::RectangleShape* shape, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y) {
    if (!freeze) {
        game.camera.toRel(&tl_x, &tl_y);
        game.camera.toRel(&bl_x, &bl_y);
        game.camera.toRel(&br_x, &br_y);

        float dx1 = tl_x-bl_x;
        float dy1 = tl_y-bl_y;
        float dx2 = br_x-bl_x;
        float dy2 = br_y-bl_y;
        float width = sqrt(dx2*dx2+dy2*dy2);
        float height = sqrt(dx1*dx1+dy1*dy1);
        float angle = atan2(dy2,dx2)*180/M_PI;

        shape->setSize(sf::Vector2f(width,height));
        shape->setPosition(tl_x,tl_y);
        shape->setRotation(angle);
    }
    window.draw(*shape);
}

Player::Player() {
    freeze = false;
    
    x = 0; y = 0;
    speed = 10;
    jumpSpeed = 11;
    gravity = 0.4f;
    gravityX = 0;
    gravityY = 0;
    pwidth = 30;
    pheight = 50;
    setOrientation(dir_up);
    shape = sf::RectangleShape();
    shape.setFillColor(sf::Color::Blue);
}

bool Player::canRotate(bool right) {
    if (freeze) return false;
    if (currentPlatform->isNull) return false;
    if (!currentPlatform->rotatable) return false;
    if (!currentPlatform->sweep(right)) return false;
    return orientation == currentPlatform->orientation;
}

void Player::rotateTo(int newOrientation) {
    vx = 0; vy = 0;
    rotateAboutPivotActual(orientation, newOrientation, currentPlatform->cx, currentPlatform->cy, &x, &y);
    setOrientation(newOrientation);
    currentPlatform->setOrientation(newOrientation);
    updateBoundaries();
}

void Player::setIsRotating(bool value) {
    freeze = value;
    currentPlatform->freeze = value;
}

void Player::draw() {
    drawRectangle(&shape,x1,y1,x1,y2,x2,y2);
}

void Player::updateBoundaries() {
    x1 = x;
    x2 = x;
    y1 = y;
    y2 = y;
    switch(orientation) {
        case dir_up:
        case dir_down:
            x1 -= pwidth/2;
            x2 += pwidth/2;
            y1 -= pheight/2;
            y2 += pheight/2;
            break;
        case dir_left:
        case dir_right:
            x1 -= pheight/2;
            x2 += pheight/2;
            y1 -= pwidth/2;
            y2 += pwidth/2;
            break;
        default:
            std::cout << "Nu\n";
    }
    
}

void Player::update(Keyboard k) {
    if (freeze) return;

    updateBoundaries();

    // Fall off the sides
    switch(this->orientation) {
        case dir_up:
        case dir_down:
            if (x1 >= currentPlatform->x2 || x2 <= currentPlatform->x1) currentPlatform = new Platform(); // Note: Memory leak?
            vx = 0; // also sneak in speed control
            break;
        case dir_left:
        case dir_right:
            if (y1 >= currentPlatform->y2 || y2 <= currentPlatform->y1) currentPlatform = new Platform();
            vy = 0;
            break;
        default:
            std::cout << "Wat\n";
    }
    
    // Do not fly off
    switch(this->orientation) {
        case dir_up: if (y2 != currentPlatform->y1) currentPlatform = new Platform(); break;
        case dir_down: if (y1 != currentPlatform->y2) currentPlatform = new Platform(); break;
        case dir_left: if (x2 != currentPlatform->x1) currentPlatform = new Platform(); break;
        case dir_right: if (x1 != currentPlatform->x2) currentPlatform = new Platform(); break;
        default: std::cout << "Wut\n";
    }
    
    if (currentPlatform->isNull) {
        vx += gravityX;
        vy += gravityY;
    }
    
    // What do left and right do
    switch(this->orientation) {
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
 
    x += vx;
    y += vy;
    
    for (int i=0;i<game.platforms.size();++i) {
        collision(&game.platforms[i]);
    }
    /*for (std::vector<Platform>::iterator it = game.platforms.begin(); it != game.platforms.end(); ++it) {
        collision(it);
    }*/
}

void Player::getGridCoordinates(int* gridX, int* gridY) {
    actualToGrid(x, y, gridX, gridY);
}

void Player::jump() {
    if (!currentPlatform->isNull) {
        currentPlatform = new Platform(); // isNull
        switch(orientation) {
            case dir_up: vy = (-1) * jumpSpeed; break;
            case dir_down: vy = jumpSpeed; break;
            case dir_left: vx = (-1) * jumpSpeed; break;
            case dir_right: vx = jumpSpeed; break;
        }
    }
}

void Player::setOrientation(int orientation) {
    this->orientation = orientation;
    
    switch(orientation) {
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
        
        switch(touchSide) {
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

Platform::Platform() {
    this->isNull = true;
}

Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, bool rotatable, int orientation) {
    freeze = false;
    
    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->leftTiles = leftTiles;
    this->rightTiles = rightTiles;
    this->rotatable = rotatable;
    setOrientation(orientation);

    shape = sf::RectangleShape();
    shape.setFillColor(sf::Color::Green);
}

void Platform::draw() {
    drawRectangle(&shape,x1,y1,x1,y2,x2,y2);
}

void Platform::update(Keyboard k) {
}

void Platform::setOrientation(int orientation) {
    this->orientation = orientation;
    
    this->x1 = x - TILE_WIDTH/2;
    this->x2 = x + TILE_WIDTH/2;
    this->y1 = y - TILE_WIDTH/2;
    this->y2 = y + TILE_WIDTH/2;
    
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
}

bool Platform::sweep(bool right) {
    int leftQuad;
    int rightQuad;
    if ((right && (orientation == dir_up || orientation == dir_down))
        || (!right && (orientation == dir_right || orientation == dir_left))) { // quads24
        switch(orientation) {
            case dir_up:
            case dir_right:
                leftQuad = (TILE_WIDTH / 2) + TILE_WIDTH * leftTiles;
                rightQuad = (TILE_WIDTH / 2) + TILE_WIDTH * rightTiles;
                break;
            case dir_down:
            case dir_left:
                leftQuad = (TILE_WIDTH / 2) + TILE_WIDTH * rightTiles;
                rightQuad = (TILE_WIDTH / 2) + TILE_WIDTH * leftTiles;
                break;
        }
        for (int i=0;i<game.platforms.size();++i) {
            if (!platCheck(x, y, leftQuad, rightQuad, false, game.platforms[i])) return false;
        }
    } else { // quads13
        switch(orientation) {
            case dir_up:
            case dir_left:
                leftQuad = (TILE_WIDTH / 2) + TILE_WIDTH * leftTiles;
                rightQuad = (TILE_WIDTH / 2) + TILE_WIDTH * rightTiles;
                break;
            case dir_down:
            case dir_right:
                leftQuad = (TILE_WIDTH / 2) + TILE_WIDTH * rightTiles;
                rightQuad = (TILE_WIDTH / 2) + TILE_WIDTH * leftTiles;
                break;
        }
        for (int i=0;i<game.platforms.size();++i) {
            if (!platCheck(x, y, leftQuad, rightQuad, true, game.platforms[i])) return false;
        }
    }
    return true;
}

bool Platform::platCheck(int pivotx, int pivoty, int leftQuad, int rightQuad, bool vertFlip, Platform other) { // vertFlip ? quads 2-4 : quads 1-3
    Point centerPivot(pivotx, pivoty);
    int centerx1 = other.x - TILE_WIDTH/2;
    int centerx2 = other.x + TILE_WIDTH/2;
    int centery1 = other.y - TILE_WIDTH/2;
    int centery2 = other.y + TILE_WIDTH/2;
    
    switch(orientation) {
        case dir_up: {
            for (int i = 0; i < other.leftTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx1 - TILE_WIDTH * i, centery1,
                                                        centerx1 - TILE_WIDTH * i, centery2, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            for (int i = 0; i < other.rightTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx2 + TILE_WIDTH * i, centery1,
                                                        centerx2 + TILE_WIDTH * i, centery2, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            break;
        }
        case dir_down: {
            for (int i = 0; i < other.leftTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx2 + TILE_WIDTH * i, centery1,
                                                        centerx2 + TILE_WIDTH * i, centery2, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            for (int i = 0; i < other.rightTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx1 - TILE_WIDTH * i, centery1,
                                                        centerx1 - TILE_WIDTH * i, centery2, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            break;
        }
        case dir_right: {
            for (int i = 0; i < other.leftTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx1, centery1 - TILE_WIDTH * i,
                                                        centerx2, centery1 - TILE_WIDTH * i, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            for (int i = 0; i < other.rightTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx1, centery2 + TILE_WIDTH * i,
                                                        centerx2, centery2 + TILE_WIDTH * i, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            break;
        }
        case dir_left: {
            for (int i = 0; i < other.leftTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx1, centery2 + TILE_WIDTH * i,
                                                        centerx2, centery2 + TILE_WIDTH * i, 
                                           leftQuad, rightQuad, vertFlip)) return false;
            }
            for (int i = 0; i < other.rightTiles + 1; ++i) {
                if (!twoPointsTwoDistances(centerPivot, centerx1, centery1 - TILE_WIDTH * i,
                                                        centerx2, centery1 - TILE_WIDTH * i, 
                                           leftQuad, rightQuad, vertFlip)) return false;
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
        if (p1x < center.x && p1y > center.y && one.distance(center) < lQuad) return false; // quad2
        if (p1x > center.x && p1y < center.y && one.distance(center) < rQuad) return false; // quad4
        if (p2x < center.x && p2y > center.y && two.distance(center) < lQuad) return false; // quad2
        if (p2x > center.x && p2y < center.y && two.distance(center) < rQuad) return false; // quad4
    } else {
        if (p1x < center.x && p1y < center.y && one.distance(center) < lQuad) return false; // quad3
        if (p1x > center.x && p1y > center.y && one.distance(center) < rQuad) return false; // quad1
        if (p2x < center.x && p2y < center.y && two.distance(center) < lQuad) return false; // quad3
        if (p2x > center.x && p2y > center.y && two.distance(center) < rQuad) return false; // quad1
    }
    return true;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

float Point::distance(Point other) {
    return sqrt(pow((float) x - (float) other.x, 2.0) + pow((float) y - (float) other.y, 2.0));
}

Door::Door() {
    this->isNull = true;
}

Door::Door(int cx, int cy, int orientation) {
    freeze = false;
    
    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->orientation = orientation;
}

void Door::draw() {

}

void Door::update(Keyboard k) {

}


Camera::Camera() {}

Camera::Camera(Player* player) {
    freeze = false;

    rotateSpeed = 0.1f;
    snapSpeed = 0.2f;

    this->orientation = 0;
    this->player = player;
    this->px = player->x;
    this->py = player->y;
    this->angle = 0;
    this->targetAngle = 0;
}

void Camera::rotateTo(int newOrientation) {
    int diff = newOrientation - orientation;
    if (diff >= 3) diff -= 4;
    if (diff < -1) diff += 4;
    orientation = newOrientation;
    std::cout << diff;
    targetAngle += M_PI/2 * diff;
    setIsRotating(true);
}

void Camera::setIsRotating(bool value) {
    rotating = value;
    player->setIsRotating(value);
}

void Camera::toRel(float* _x, float* _y) {
    float dx = *_x - px;
    float dy = *_y - py;
    float theta = angle - atan2(dy,dx);
    float length = sqrt(dx*dx+dy*dy);

    *_x = length*cos(theta) + RES_X/2;
    *_y = RES_Y/2 - length*sin(theta);
}

void Camera::draw() {

}

void Camera::update(Keyboard k) {
    float dx = player->x - px;
    float dy = player->y - py;
    px += dx*snapSpeed;
    py += dy*snapSpeed;

    if (!rotating) return;
    //std::cout << angle << " " << targetAngle << "\n";
    if (angle < targetAngle) {
        angle += rotateSpeed;
        if (angle >= targetAngle) {
            onReach();
        }
    } else {
        angle -= rotateSpeed;
        if (angle < targetAngle) {
            onReach();
        }
    }
}

void Camera::onReach() {
    while (targetAngle < 0) targetAngle += 2*M_PI;
    while (targetAngle > M_PI) targetAngle -= 2*M_PI;
    angle = targetAngle;
    setIsRotating(false);
}