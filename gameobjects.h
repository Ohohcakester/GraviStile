#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include "keyboard.h"

enum {
    dir_up,
    dir_right,
    dir_down,
    dir_left
};

class IGameObject {
public:
    int x;
    int y;

    virtual ~IGameObject() {}
    virtual void draw() = 0;
    virtual void update(Keyboard k) = 0;

    void drawCircle(sf::CircleShape shape, int px, int py);
    void drawRectangle(sf::RectangleShape shape, int tl_x, int tl_y, int bl_x, int bl_y, int br_x, int br_y);
};

class Platform : public IGameObject {
public:
    sf::RectangleShape shape;

    bool isNull;
    int cx;
    int cy;
    int leftTiles;
    int rightTiles;
    bool rotatable;
    int orientation;
float a;
    Platform();

    Platform(int pivotX, int pivotY, int leftTiles, int rightTiles, bool rotatable, int orientation);

    virtual void draw();

    virtual void update(Keyboard k);
};

class Door : public IGameObject {
public:
    bool isNull;
    int cx;
    int cy;
    bool rotatable;
    int orientation;
    
    Door();
    
    Door(int cx, int cy, int orientation);

    virtual void draw();

    virtual void update(Keyboard k);

};


class Player : public IGameObject {
public:
    sf::CircleShape shape;
    float vx;
    float vy;
    float ax;
    float ay;
    int speed;
    int jumpSpeed;
    float gravity;
    int orientation;
    Platform currentPlatform;

    Player();

    virtual void draw();

    virtual void update(Keyboard k);

    virtual void jump();
    
    void getGridCoordinates(int* gridX, int* gridY);
};

class Camera : public IGameObject {
public:
    float rotateSpeed;
    float snapSpeed;

    float px, py;
    float angle;
    float targetAngle;
    int orientation;
    bool rotating;
    Player* player;

    Camera();

    Camera(Player* player);

    float toRelX(float _x);
    float toRelY(float _y);

    void rotateTo(int newOrientation);

    virtual void draw();

    virtual void update(Keyboard k);

    void onReach();
};


#endif