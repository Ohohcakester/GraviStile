#include <SFML/Graphics.hpp>
#include "globals.h"

GameGlobals game;

int RES_X = 800;
int RES_Y = 600;
int TILE_WIDTH = 50;

sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "SFML works!");



void gridToActual(int gridX, int gridY, int* actualX, int* actualY) {
    *actualX = gridX*TILE_WIDTH + TILE_WIDTH/2;
    *actualY = gridY*TILE_WIDTH + TILE_WIDTH/2;
}

void actualToGrid(int actualX, int actualY, int* gridX, int* gridY) {
    *gridX = (actualX-TILE_WIDTH/2)/TILE_WIDTH;
    *gridY = (actualY-TILE_WIDTH/2)/TILE_WIDTH;
}