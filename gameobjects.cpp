#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "gameobjects.h"
#include "keyboard.h"
#include "globals.h"


void IGameObject::drawCircle(sf::CircleShape shape, int px, int py) {
    px = game.camera.toRelX(px);
    py = game.camera.toRelY(py);
    shape.setOrigin(-px+shape.getRadius(),-py+shape.getRadius());
    window.draw(shape);
}

void IGameObject::drawRectangle(sf::RectangleShape shape, int tl_x, int tl_y, int bl_x, int bl_y, int br_x, int br_y) {
    tl_x = game.camera.toRelX(tl_x);
    tl_y = game.camera.toRelY(tl_y);
    bl_x = game.camera.toRelX(bl_x);
    bl_y = game.camera.toRelY(bl_y);
    br_x = game.camera.toRelX(br_x);
    br_y = game.camera.toRelY(br_y);

    float dx1 = tl_x-bl_x;
    float dy1 = tl_y-bl_y;
    float dx2 = br_x-bl_x;
    float dy2 = br_y-bl_y;
    float width = sqrt(dx2*dx2+dy2*dy2);
    float height = sqrt(dx1*dx1+dy1*dy1);
    float angle = atan2(dy2,dx2)*180/M_PI;

    shape.setSize(sf::Vector2f(width,height));
    shape.setPosition(tl_x,tl_y);
    shape.setRotation(angle);
    window.draw(shape);
}

Player::Player() {
    x = 0; y = 0;
    speed = 10;
    jumpSpeed = 11;
    gravity = 0.4f;
    shape = sf::CircleShape(50);
    shape.setFillColor(sf::Color::Green);
}

void Player::draw() {
    drawCircle(shape, x, y);
}

void Player::update(Keyboard k) {
    vx = 0;
    if (currentPlatform.isNull) vy += gravity;
    if (k.left) vx -= speed;
    if (k.right) vx += speed;
 
    x += vx;
    y += vy;
    
    for (std::vector<Platform>::iterator it = game.platforms.begin(); it != game.platforms.end(); ++it) {
        collision(*it);
    }
}

void Player::getGridCoordinates(int* gridX, int* gridY) {
    actualToGrid(x, y, gridX, gridY);
}

void Player::jump() {
    currentPlatform = Platform(); // isNull
    std::cout<<"jumping\n";
    vy = (-1) * speed;
}

void Player::collision(Platform plat) {
    int x1 = x - 50;
    int x2 = x + 50;
    int y1 = y + 50;
    int y2 = y - 50;
    
    if (x2 > plat.x1 && x1 < plat.x2 && y2 > plat.y1 && y1 < plat.y2) {
        std::cout << "COLLIDING";
        int touchSide = -1;
        int closestDist = TILE_WIDTH * 2;
        if (x2 - plat.x1 < closestDist) {
            touchSide = dir_left;
            closestDist = x2 - plat.x1;
        }
        if (plat.x2 - x1 < closestDist) {
            touchSide = dir_right;
            closestDist = plat.x2 - x1;
        }
        if (y2 - plat.y1 < closestDist) {
            touchSide = dir_up;
            closestDist = y2 - plat.y1;
        }
        if (plat.y2 - y1 < closestDist) {
            touchSide = dir_down;
            closestDist = plat.y2 - y1;
        }
        
        switch(touchSide) {
            case dir_left: {
                x -= closestDist;
                vx = 0;
                break;
            }
            case dir_right: {
                x += closestDist;
                vx = 0;
                break;
            }
            case dir_up: {
                y -= closestDist;
                vy = 0;
                break;
            }
            case dir_down: {
                y += closestDist;
                vy = 0;
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
    drawRectangle(shape,x1,y1,x1,y2,x2,y2);
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

Door::Door() {
    this->isNull = true;
}

Door::Door(int cx, int cy, int orientation) {
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
    rotateSpeed = 0.1f;
    snapSpeed = 0.2f;

    this->player = player;
    this->px = player->x;
    this->py = player->y;
}

void Camera::rotateTo(int newOrientation) {
    int diff = newOrientation - orientation;
    if (diff >= 3) diff -= 3;
    if (diff < -1) diff += 3;
    orientation = newOrientation;
    targetAngle += M_PI/2 * diff;
    rotating = true;
}

float Camera::toRelX(float _x) {
    return _x - px + RES_X/2;
}

float Camera::toRelY(float _y) {
    return _y - py + RES_Y/2;
}

void Camera::draw() {

}

void Camera::update(Keyboard k) {
    float dx = player->x - px;
    float dy = player->y - py;
    px += dx*snapSpeed;
    py += dy*snapSpeed;

    if (!rotating) return;
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
    rotating = false;
}