#include "Laser.h"
#include "LaserSource.h"
#include "globals.h"
#include "Orientations.h"
#include "Textures.h"
#include "globals.h"
#include "Sfx.h"
#include "GameGlobals.h"
#include "gamemath.h"
#include <iostream>

const int halfWidth = 20 / 2;
const int quarterWidth = 20 / 4;

Laser::Laser() {
}

Laser::Laser(LaserSource* source) : source(source) {
    shape = sf::RectangleShape();
    shape.setFillColor(global::textures->laserColor);
}

void Laser::setup(int orientation, int sx, int sy, int ex, int ey) {
    this->orientation = orientation;
    int rsx, rsy, rex, rey;
    gridToActual(sx, sy, &rsx, &rsy);
    gridToActual(ex, ey, &rex, &rey);
    int halfTileWidth = global::TILE_WIDTH / 2;

    switch (orientation) {
    case dir_up:
        x1 = rsx - halfWidth;
        x2 = rsx + halfWidth;
        y1 = rey + halfTileWidth;
        y2 = rsy - halfTileWidth;
        targetX = rsx;
        targetY = y1;
        break;
    case dir_right:
        y1 = rsy - halfWidth;
        y2 = rsy + halfWidth;
        x1 = rsx + halfTileWidth;
        x2 = rex - halfTileWidth;
        targetX = x2;
        targetY = rsy;
        break;
    case dir_down:
        x1 = rsx - halfWidth;
        x2 = rsx + halfWidth;
        y1 = rsy + halfTileWidth;
        y2 = rey - halfTileWidth;
        targetX = rsx;
        targetY = y2;
        break;
    case dir_left:
        y1 = rsy - halfWidth;
        y2 = rsy + halfWidth;
        x1 = rex + halfTileWidth;
        x2 = rsx - halfTileWidth;
        targetX = x1;
        targetY = rsy;
        break;
    }
}

void Laser::draw() {
    if (!this->active) return;
    drawRectangle(&shape, x1, y1, x1, y2, x2, y2);
}

void Laser::update(Keyboard k) {
    if (this->active) {
        if (particleTimer <= 0) {
            float vx = 0, vy = randRange(1.0f,1.5f);
            float ax = 0, ay = 0.2f;
            rotateVector(&vx, &vy, randomAngle());
            rotateVector(&ax, &ay, orientationToAngle(this->orientation));

            int px, py;
            switch (orientation) {
            case dir_up:
            case dir_down:
                px = targetX + randRange(-quarterWidth, quarterWidth);
                py = targetY;
                break;
            case dir_left:
            case dir_right:
                px = targetX;
                py = targetY + randRange(-quarterWidth, quarterWidth);
                break;
            }

            global::game.spawnNewSfx(new sfx::LaserParticle(px, py, vx, vy, ax, ay));

            particleTimer = rand() % 3 + 2;
        }
        particleTimer--;
    }

}