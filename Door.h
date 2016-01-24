#ifndef DOOR_H_
#define DOOR_H_

#include <SFML/Graphics.hpp>
#include "IGameObject.h"
#include "keyboard.h"
#include "IClickable.h"

class Door : public IGameObject, public IClickable {
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

    void draw();

    void update(Keyboard k);

    bool isWithinClickHitbox(int x, int y);
};



#endif