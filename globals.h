#ifndef GLOBALS_H
#define GLOBALS_H

#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
class GameGlobals;
class Menu;
class Textures;
class Keyboard;
class GameStats;

namespace editor {
    class EditorState;
}


namespace global {
    extern GameGlobals game;
    extern Keyboard key;
    extern GameStats gameStats;
    extern editor::EditorState editorState;
    extern Textures* textures;
    extern Menu menu;

    extern int RES_X;
    extern int RES_Y;
    extern int TILE_WIDTH;
    extern const int OUT_OF_BOUNDS_TILES;

    extern const float ROTATE_SPEED;
    extern const int NUMBER_OF_STAGES;

    extern sf::RenderWindow* window;
}

void gridToActual(int gridX, int gridY, int* actualX, int* actualY);
void actualToGrid(int actualX, int actualY, int* gridX, int* gridY);

void rotateAboutPivotActual(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y);
void rotateAboutPivotGrid(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y);

#endif