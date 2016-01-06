#ifndef GLOBALS_H
#define GLOBALS_H

#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
class GameGlobals;
class Menu;
class Textures;



extern GameGlobals game;
extern Textures* textures;
extern Menu menu;

extern int RES_X;
extern int RES_Y;
extern int TILE_WIDTH;
extern const int OUT_OF_BOUNDS_TILES;

extern sf::RenderWindow* window;

void gridToActual(int gridX, int gridY, int* actualX, int* actualY);
void actualToGrid(int actualX, int actualY, int* gridX, int* gridY);

void rotateAboutPivotActual(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y);
void rotateAboutPivotGrid(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y);


#endif