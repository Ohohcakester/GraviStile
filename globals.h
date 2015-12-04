#ifndef GLOBALS_H
#define GLOBALS_H
#include "gameobjects.h"
#include "keyboard.h"

class GameGlobals {
public:
    Player player;
    Keyboard key;


    int nTilesX;
    int nTilesY;
    int width;
    int height;
};

extern GameGlobals game;

extern int RES_X;
extern int RES_Y;
extern int TILE_WIDTH;

extern sf::RenderWindow window;

void gridToActual(int gridX, int gridY, int* actualX, int* actualY);
void actualToGrid(int actualX, int actualY, int* gridX, int* gridY);






#endif