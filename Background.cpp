
#include "globals.h"
#include "GameGlobals.h"
#include "Textures.h"
#include "Background.h"

Background::Background() {}

Background::Background(int x, int y) {
    gridToActual(x, y, &this->x, &this->y);
    int nTiles = std::max(global::game.grid.sizeX, global::game.grid.sizeY);
    this->width = nTiles*global::TILE_WIDTH * 2;
    this->height = nTiles*global::TILE_WIDTH * 3 / 2;
    sprite.setTexture(global::textures->background);
}

void Background::draw() {
    drawSprite(&sprite, x - width, y - height, x - width, y + height, x + width, y + height);
}

void Background::update(Keyboard k) {
}