#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include "keyboard.h"


class IGameObject {
public:
    int x;
    int y;

    virtual ~IGameObject() {}
    virtual void draw() = 0;
    virtual void update(Keyboard k) = 0;

    void drawCircle(sf::CircleShape shape, int px, int py);
};

class Player : public IGameObject {
public:
    sf::CircleShape shape;
    int vx;
    int vy;
    int speed;

    Player();

    virtual void draw();

    virtual void update(Keyboard k);
};




#endif