#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics.hpp>
#include "IGameObject.h"
#include "Platform.h"

class Player : public IGameObject {
public:
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

    virtual void draw();

    virtual void update(Keyboard k);

    virtual void jump();

    bool canRotate(bool right);

    void rotateTo(int orientation);

    void updateBoundaries();

    void Player::getSpriteCoordinates(float* sx, float* sy);

    void getGridCoordinates(int* gridX, int* gridY);

    void collision(Platform* plat);

    void setOrientation(int orientation);


    bool rotatesIntoPlatform(bool rotateRight);
    bool collidesWith(float _x1, float _y1, float _x2, float _y2, Platform* plat);
};


#endif
