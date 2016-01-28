#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics.hpp>
#include "IGameObject.h"
#include "Platform.h"
#include "IClickable.h"

class Player : public IGameObject, public IClickable {
public:
    bool isActive;

    sf::Sprite sprite;
    sf::RectangleShape shape;
    float vx;
    float vy;
    int speed;
    int jumpSpeed;
    float gravity;
    float gravityX;
    float gravityY;
    int orientation;
    int targetOrientation;
    int pwidth;
    int pheight;
    Platform* currentPlatform;
    Platform* frozenPlatform;
    Platform nullPlatform;
    
    float angle;
    bool isRotating;

    bool facingRight;

    // rect
    int x1;
    int y1;
    int x2;
    int y2;

    float rotatingBaseDX;
    float rotatingBaseDY;

    Player();

    Player(int cx, int cy);

    void draw();

    void update(Keyboard k);

    bool isWithinClickHitbox(int x, int y);

    void jump();

    bool canRotate(bool right);

    void finishRotating(bool success);

    void rotateTo(int orientation);

    void updateBoundaries();

    void tryMoveInDirection(int dx, int dy);

    int getPlatformMoveDistance(int fromX, int fromY, int dx, int dy, Platform* plat, int* moveX, int* moveY, int* hitDirection);

    void Player::getSpriteCoordinates(float* sx, float* sy);

    void getGridCoordinates(int* gridX, int* gridY);

    void collision(Platform* plat);

    void setOrientation(int orientation);

    void setCurrentPlatform(Platform* platform);

    void snapToCurrentPlatformHeight();

    bool rotatesIntoPlatform();
    bool collidesWithTargetOrientation(float _x1, float _y1, float _x2, float _y2, Platform* plat);

    bool isControlsDisabled();
    void die();
    bool isTouchingLaser();
};


#endif
