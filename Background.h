#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <SFML/Graphics.hpp>
#include "IGameObject.h"

class Background : public IGameObject {
public:
    sf::Sprite sprite;
    float width;
    float height;

    Background();

    Background(int x, int y);

    virtual void draw();

    virtual void update(Keyboard k);
};


#endif