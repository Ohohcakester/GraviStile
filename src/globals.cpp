#include <SFML/Graphics.hpp>
#include <iostream>
#include "globals.h"
#include "GameGlobals.h"
#include "Textures.h"
#include "Menu.h"
#include "gamemath.h"
#include "EditorState.h"
#include "GameStats.h"

void gridToActual(int gridX, int gridY, int* actualX, int* actualY) {
    *actualX = gridX*global::TILE_WIDTH + global::TILE_WIDTH / 2;
    *actualY = gridY*global::TILE_WIDTH + global::TILE_WIDTH / 2;
}

void actualToGrid(int actualX, int actualY, int* gridX, int* gridY) {
    *gridX = div_floor(actualX, global::TILE_WIDTH);
    *gridY = div_floor(actualY, global::TILE_WIDTH);
}

void rotateClockwise(int cx, int cy, int* x, int* y) {
    int dx = *x-cx;
    int dy = *y-cy;
    *x = cx-dy;
    *y = cy+dx;
}

void rotateAboutPivot(int oldOrientation, int newOrientation, int cx, int cy, int* x, int* y) {
    int clockwiseRotations = newOrientation - oldOrientation;
    if (clockwiseRotations < 0) clockwiseRotations += 4;
    for (int i=0;i<clockwiseRotations;++i) {
        rotateClockwise(cx,cy,x,y);
    }
}

void rotateAboutPivotActual(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y) {
    int cx, cy;
    gridToActual(gridcx, gridcy, &cx, &cy);
    rotateAboutPivot(oldOrientation, newOrientation, cx, cy, x, y);
}

void rotateAboutPivotGrid(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y) {
    rotateAboutPivot(oldOrientation, newOrientation, gridcx, gridcy, x, y);
}

namespace global {
    int RES_X = 800;
    int RES_Y = 600;
    int TILE_WIDTH = 50;
    const int OUT_OF_BOUNDS_TILES = 10;

    const float ROTATE_SPEED = 0.1f;

    const int NUMBER_OF_STAGES = 24;


    GameGlobals game;
    GameStats gameStats;
    Keyboard key;
    editor::EditorState editorState;
    Textures* textures;
    Menu menu;


    sf::RenderWindow* window;
}