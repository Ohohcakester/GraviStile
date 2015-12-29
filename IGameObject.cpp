#include "globals.h"
#include "IGameObject.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

void IGameObject::drawCircle(sf::CircleShape* shape, float px, float py) {
    if (!freeze) {
        game.camera.toRel(&px, &py);

        shape->setOrigin(-px + shape->getRadius(), -py + shape->getRadius());
    }
    window->draw(*shape);
}

void IGameObject::drawRectangle(sf::RectangleShape* shape, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y) {
    if (!freeze) {
        game.camera.toRel(&tl_x, &tl_y);
        game.camera.toRel(&bl_x, &bl_y);
        game.camera.toRel(&br_x, &br_y);

        float dx1 = tl_x - bl_x;
        float dy1 = tl_y - bl_y;
        float dx2 = br_x - bl_x;
        float dy2 = br_y - bl_y;
        float width = sqrt(dx2*dx2 + dy2*dy2);
        float height = sqrt(dx1*dx1 + dy1*dy1);
        float angle = atan2(dy2, dx2) * 180 / M_PI;

        shape->setSize(sf::Vector2f(width, height));
        shape->setPosition(tl_x, tl_y);
        shape->setRotation(angle);
    }
    window->draw(*shape);
}

void IGameObject::drawPlayerSprite(sf::Sprite* sprite, float x1, float y1, float x2, float y2, bool facingRight) {
    if (!freeze) {
        game.camera.toRel(&x1, &y1);
        game.camera.toRel(&x2, &y2);

        float width = sprite->getLocalBounds().width;
        float height = sprite->getLocalBounds().height;

        float dx = abs(x2 - x1) / width;
        float dy = abs(y2 - y1) / height;
        float fit = std::min(dx, dy);

        if (facingRight) {
            sprite->setScale(sf::Vector2f(fit, fit));
            sprite->setPosition(std::min(x1, x2), std::min(y1, y2));
        }
        else {
            sprite->setScale(sf::Vector2f(-fit, fit));
            sprite->setPosition(std::max(x1, x2), std::min(y1, y2));
        }
    }
    window->draw(*sprite);
}

void IGameObject::drawSprite(sf::Sprite* sprite, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y) {
    if (!freeze) {
        game.camera.toRel(&tl_x, &tl_y);
        game.camera.toRel(&bl_x, &bl_y);
        game.camera.toRel(&br_x, &br_y);

        float dx1 = tl_x - bl_x;
        float dy1 = tl_y - bl_y;
        float dx2 = br_x - bl_x;
        float dy2 = br_y - bl_y;
        float width = sqrt(dx2*dx2 + dy2*dy2);
        float height = sqrt(dx1*dx1 + dy1*dy1);
        float angle = atan2(dy2, dx2) * 180 / M_PI;

        float spriteWidth = sprite->getLocalBounds().width;
        float spriteHeight = sprite->getLocalBounds().height;

        float dx = width / spriteWidth;
        float dy = height / spriteHeight;
        float fit = std::min(dx, dy);

        sprite->setScale(sf::Vector2f(fit, fit));
        sprite->setPosition(tl_x, tl_y);
        sprite->setRotation(angle);
    }
    window->draw(*sprite);
}
