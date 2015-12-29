
#include "globals.h"
#include "Background.h"

Background::Background() {}

Background::Background(int x, int y) {
    gridToActual(x, y, &this->x, &this->y);
    int nTiles = std::max(game.nTilesX, game.nTilesY);
    this->width = nTiles*TILE_WIDTH * 2;
    this->height = nTiles*TILE_WIDTH * 3 / 2;
    sprite.setTexture(textures->background);
}

void Background::draw() {
    drawSprite(&sprite, x - width, y - height, x - width, y + height, x + width, y + height);
}

void Background::update(Keyboard k) {
}