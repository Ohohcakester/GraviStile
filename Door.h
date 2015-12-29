#ifndef DOOR_H_
#define DOOR_H_

#include <SFML/Graphics.hpp>
#include "IGameObject.h"
#include "keyboard.h"

class Door : public IGameObject {
public:
    sf::RectangleShape shape;
    sf::Sprite sprite;

    bool isNull;
    int cx;
    int cy;
    bool rotatable;
    int orientation;
    int endStageTimeout;

    Door();

    Door(int cx, int cy, int orientation);

    virtual void draw();

    virtual void update(Keyboard k);

};



#endif