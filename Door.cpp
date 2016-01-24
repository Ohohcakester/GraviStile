#include <iostream>
#include "globals.h"
#include "GameGlobals.h"
#include "Textures.h"
#include "Door.h"
#include "gamemath.h"

Door::Door() {
    this->isNull = true;
}

Door::Door(int cx, int cy, int orientation) {
    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->orientation = orientation;

    sprite.setTexture(textures->door);

    shape = sf::RectangleShape();
    shape.setFillColor(sf::Color::Yellow);

    this->endStageTimeout = 60;
}

void Door::draw() {
    float w = TILE_WIDTH * 1 / 2;

    switch (orientation) {
    case dir_up:
        drawSprite(&sprite, x - w, y - w, x - w, y + w, x + w, y + w);
        break;
    case dir_right:
        drawSprite(&sprite, x + w, y - w, x - w, y - w, x - w, y + w);
        break;
    case dir_down:
        drawSprite(&sprite, x + w, y + w, x + w, y - w, x - w, y - w);
        break;
    case dir_left:
        drawSprite(&sprite, x - w, y + w, x + w, y + w, x + w, y - w);
        break;
    }

    //drawRectangle(&shape,x1,y1,x1,y2,x2,y2);
    //drawSprite(&sprite,x1,y1,x1,y2,x2,y2);
}

void Door::update(Keyboard k) {
    if (game.player.isRotating) return;
    if (game.player.currentPlatform->isNull) return;
    if (!game.player.currentPlatform->isUnderDoor(cx, cy, orientation)) return;
    if (game.player.orientation != orientation) return;
    int playerX, playerY;
    game.player.getGridCoordinates(&playerX, &playerY);
    if (playerX == cx && playerY == cy) {
        game.puzzleComplete = true;
    }
}

bool Door::isWithinClickHitbox(int sx, int sy) {
    float w = TILE_WIDTH * 1 / 2;
    int x1 = x - w;
    int y1 = y - w;
    int x2 = x + w;
    int y2 = y + w;

    return isWithinRect(sx, sy, x1, y1, x2, y2);
}