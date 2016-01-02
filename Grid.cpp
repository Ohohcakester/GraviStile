#include <iostream>
#include <vector>
#include "Orientations.h"
#include "Grid.h"
#include "Platform.h"

Grid::Grid() {
    _isNull = true;
    this->blocked = NULL;
}

Grid::Grid(int minX, int minY, int maxX, int maxY) :
        minX(minX), minY(minY), maxX(maxX), maxY(maxY) {
    this->_isNull = false;
    this->sizeX = maxX - minX;
    this->sizeY = maxY - minY;
    this->size = sizeX*sizeY;
    this->blocked = new bool[size];
}

Grid::~Grid() {
    delete blocked;
}

bool Grid::isBlocked(int x, int y) {
    toGridCoordinates(&x, &y);
    if (x < 0 || y < 0 || x >= sizeX || y >= sizeY) return false;
    return blocked[y*sizeX + x];
}

void Grid::setBlocked(int x, int y, bool value) {
    toActualCoordinates(&x, &y);
    if (x < 0 || y < 0 || x >= sizeX || y >= sizeY) std::cout << "ERROR: SETTING GRID OUT OF RANGE: (" << x << ", " << y << ")\n";
    blocked[y*sizeX + x] = value;
}

void Grid::reset() {
    for (int i = 0; i < size; ++i) {
        blocked[i] = false;
    }
}

void Grid::addPlatforms(std::vector<Platform*>* platforms) {
    int len = platforms->size();
    for (int i = 0; i < len; ++i) {
        addPlatform((*platforms)[i]);
    }
}

void Grid::addPlatform(Platform* platform) {
    switch (platform->orientation) {
    case dir_up: {
        int minX = platform->cx - platform->leftTiles;
        int maxX = platform->cx + platform->rightTiles;
        int y = platform->cy;
        for (int x = minX; x <= maxX; ++x) setBlocked(x, y, true);
        break;
    }
    case dir_right: {
        int minY = platform->cy - platform->rightTiles;
        int maxY = platform->cy + platform->leftTiles;
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
        int minY = platform->cy - platform->leftTiles;
        int maxY = platform->cy + platform->rightTiles;
        int x = platform->cx;
        for (int y = minY; y <= maxY; ++y) setBlocked(x, y, true);
        break;
    }

    }
}

void Grid::toGridCoordinates(int* x, int* y) {
    x -= minX;
    y -= minY;
}

void Grid::toActualCoordinates(int* x, int* y) {
    x += minX;
    y += minY;
}

