#include <iostream>
#include <vector>
#include "Orientations.h"
#include "Grid.h"
#include "Platform.h"
#include "globals.h"

Grid::Grid() {
    _isNull = true;
}

Grid::Grid(int minX, int minY, int maxX, int maxY) :
        minX(minX), minY(minY), maxX(maxX), maxY(maxY) {
    this->_isNull = false;
    this->sizeX = maxX - minX + 1;
    this->sizeY = maxY - minY + 1;
    this->size = sizeX*sizeY;
    this->blocked.resize(size);

    gridToActual(maxX, maxY, &this->boundMaxX, &this->boundMaxY);
    gridToActual(minX, minY, &this->boundMinX, &this->boundMinY);
    this->boundMaxX += OUT_OF_BOUNDS_TILES * TILE_WIDTH;
    this->boundMaxY += OUT_OF_BOUNDS_TILES * TILE_WIDTH;
    this->boundMinX -= OUT_OF_BOUNDS_TILES * TILE_WIDTH;
    this->boundMinY -= OUT_OF_BOUNDS_TILES * TILE_WIDTH;
}

bool Grid::isBlocked(int x, int y) {
    toGridCoordinates(&x, &y);
    if (x < 0 || y < 0 || x >= sizeX || y >= sizeY) return false;
    return blocked[y*sizeX + x];
}

void Grid::setBlocked(int x, int y, bool value) {
    toGridCoordinates(&x, &y);
    if (x < 0 || y < 0 || x >= sizeX || y >= sizeY) std::cout << "ERROR: SETTING GRID OUT OF RANGE: (" << x << ", " << y << ")\n";
    blocked[y*sizeX + x] = value;
}

void Grid::reset() {
    for (int i = 0; i < size; ++i) {
        blocked[i] = false;
    }
}

void Grid::addPlatforms(std::vector<Platform*>* platforms) {
    for (size_t i = 0, n = platforms->size(); i < n; ++i) {
        addPlatform((*platforms)[i]);
    }
}

void Grid::addPlatform(Platform* platform) {
    if (platform->isDisabled()) return;
    switch (platform->orientation) {
    case dir_up: {
        int minX = platform->cx - platform->leftTiles;
        int maxX = platform->cx + platform->rightTiles;
        int y = platform->cy;
        for (int x = minX; x <= maxX; ++x) setBlocked(x, y, true);
        break;
    }
    case dir_right: {
        int minY = platform->cy - platform->leftTiles;
        int maxY = platform->cy + platform->rightTiles;
        int x = platform->cx;
        for (int y = minY; y <= maxY; ++y) setBlocked(x, y, true);
        break;
    }
    case dir_down: {
        int minX = platform->cx - platform->rightTiles;
        int maxX = platform->cx + platform->leftTiles;
        int y = platform->cy;
        for (int x = minX; x <= maxX; ++x) setBlocked(x, y, true);
        break;
    }
    case dir_left: {
        int minY = platform->cy - platform->rightTiles;
        int maxY = platform->cy + platform->leftTiles;
        int x = platform->cx;
        for (int y = minY; y <= maxY; ++y) setBlocked(x, y, true);
        break;
    }

    }
}

void Grid::toGridCoordinates(int* x, int* y) {
    *x -= minX;
    *y -= minY;
}

void Grid::toActualCoordinates(int* x, int* y) {
    *x += minX;
    *y += minY;
}

bool Grid::isOutOfBounds(int x, int y) {
    return x > boundMaxX || y > boundMaxY || x < boundMinX || y < boundMinY;
}

