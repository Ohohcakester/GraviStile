#include "Laser.h"
#include "LaserSource.h"
#include "globals.h"
#include "Orientations.h"

int halfWidth = 30/2;

Laser::Laser() {
}

Laser::Laser(LaserSource* source) : source(source) {
}

void Laser::setup(int orientation, int sx, int sy, int ex, int ey) {
    this->orientation = orientation;

    switch (orientation) {
    case dir_up:
        x1 = sx - halfWidth;
        x2 = sx + halfWidth;
        y1 = ey + TILE_WIDTH / 2;
        y2 = sy - TILE_WIDTH / 2;
        break;
    case dir_right:
        y1 = sy - halfWidth;
        y2 = sy + halfWidth;
        x1 = sx + TILE_WIDTH / 2;
        x2 = ex - TILE_WIDTH / 2;
        break;
    case dir_down:
        x1 = sx - halfWidth;
        x2 = sx + halfWidth;
        y1 = sy + TILE_WIDTH / 2;
        y2 = ey - TILE_WIDTH / 2;
        break;
    case dir_left:
        y1 = sy - halfWidth;
        y2 = sy + halfWidth;
        x1 = ex + TILE_WIDTH / 2;
        x2 = sx - TILE_WIDTH / 2;
        break;
    }
}

void Laser::draw() {

}
void Laser::update(Keyboard k) {

}