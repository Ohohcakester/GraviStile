#include "globals.h"
#include "IGameObject.h"
#include "Camera.h"
#include "GameGlobals.h"

#define _USE_MATH_DEFINES
#include <math.h>

void IGameObject::drawCircle(sf::CircleShape* shape, float px, float py) {
    //if (!freeze) {
    game.camera.toRel(&px, &py);

    shape->setOrigin(-px + shape->getRadius(), -py + shape->getRadius());
    //}
    window->draw(*shape);
}

void IGameObject::drawRectangle(sf::RectangleShape* shape, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y) {
    //if (!freeze) {
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
    //}
    window->draw(*shape);
}

void IGameObject::drawSprite(sf::Sprite* sprite, float tl_x, float tl_y, float bl_x, float bl_y, float br_x, float br_y, bool xFlipped) {
    //if (!freeze) {
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

    float fit = height / spriteHeight;
    float newWidthRatio = height / width * spriteWidth / spriteHeight;

    if (xFlipped) {
        sprite->setScale(sf::Vector2f(-fit, fit));
        sprite->setPosition(tl_x + dx2*0.5f*(1 + newWidthRatio), tl_y + dy2*0.5f*(1 + newWidthRatio));
    } else {
        sprite->setScale(sf::Vector2f(fit, fit));
        sprite->setPosition(tl_x + dx2*0.5f*(1 - newWidthRatio), tl_y + dy2*0.5f*(1 - newWidthRatio));
    }
    sprite->setRotation(angle);
    //}
    window->draw(*sprite);
}