#ifndef I_GAME_OBJECT_H_
#define I_GAME_OBJECT_H_

#include <SFML/Graphics.hpp>
#include "Orientations.h"
#include "keyboard.h"

class IGameObject {
public:
    int x;
    int y;

    virtual ~IGameObject() {}
    virtual void draw() = 0;
    virtual void update(Keyboard k) = 0;

    void drawCircle(sf::CircleShape* shape, float px, float py);
    void drawRectangle(sf::RectangleShape* shape, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y);
    void drawSprite(sf::Sprite* sprite, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y, bool xFlipped = false);
};


#endif