#include "Laser.h"
#include "LaserSource.h"
#include "globals.h"
#include "Orientations.h"
#include "Textures.h"
#include "globals.h"

int halfWidth = 30/2;

Laser::Laser() {
}

Laser::Laser(LaserSource* source) : source(source) {
    shape = sf::RectangleShape();
    shape.setFillColor(textures->laserColor);
}

void Laser::setup(int orientation, int sx, int sy, int ex, int ey) {
    this->orientation = orientation;
    int rsx, rsy, rex, rey;
    gridToActual(sx, sy, &rsx, &rsy);
    gridToActual(ex, ey, &rex, &rey);

    switch (orientation) {
    case dir_up:
        x1 = rsx - halfWidth;
        x2 = rsx + halfWidth;
        y1 = rey + TILE_WIDTH / 2;
        y2 = rsy - TILE_WIDTH / 2;
        break;
    case dir_right:
        y1 = rsy - halfWidth;
        y2 = rsy + halfWidth;
        x1 = rsx + TILE_WIDTH / 2;
        x2 = rex - TILE_WIDTH / 2;
        break;
    case dir_down:
        x1 = rsx - halfWidth;
        x2 = rsx + halfWidth;
        y1 = rsy + TILE_WIDTH / 2;
        y2 = rey - TILE_WIDTH / 2;
        break;
    case dir_left:
        y1 = rsy - halfWidth;
        y2 = rsy + halfWidth;
        x1 = rex + TILE_WIDTH / 2;
        x2 = rsx - TILE_WIDTH / 2;
        break;
    }
}

void Laser::draw() {
    if (!this->active) return;
    drawRectangle(&shape, x1, y1, x1, y2, x2, y2);
}

void Laser::update(Keyboard k) {

}