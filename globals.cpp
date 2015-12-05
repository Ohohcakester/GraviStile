#include <SFML/Graphics.hpp>
#include <iostream>
#include "globals.h"

GameGlobals game;
Textures textures;
Menu menu;

int RES_X = 800;
int RES_Y = 600;
int TILE_WIDTH = 50;

sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "SFML In Space");

Textures::Textures() {
    if (!background.loadFromFile("img/bg.jpg")) {
        std::cout << "Unable to load background!";
    }
    if (!player.loadFromFile("img/player.png")) {
        std::cout << "Unable to load player!";
    }
    if (!door.loadFromFile("img/door.png")) {
        std::cout << "Unable to load door!";
    }
    if (!pivot.loadFromFile("img/pivot.png")) {
        std::cout << "Unable to load pivot!";
    }

    platformColor = sf::Color(110,63,75,255);
    platformSurfaceColor = sf::Color(255,191,196,255);
    //std::cout << platformColor.r << " " << platformColor.g << " " << platformColor.b << std::endl;
}


Menu::Menu() {
    cols = 5;
    nItems = 15;
    selection = 0;
}

void Menu::next() {
    selection++;
    if (selection >= nItems) selection = 0;
}

void Menu::previous() {
    selection--;
    if (selection < 0) selection = nItems - 1;
}


void gridToActual(int gridX, int gridY, int* actualX, int* actualY) {
    *actualX = gridX*TILE_WIDTH + TILE_WIDTH/2;
    *actualY = gridY*TILE_WIDTH + TILE_WIDTH/2;
}

void actualToGrid(int actualX, int actualY, int* gridX, int* gridY) {
    *gridX = (actualX-TILE_WIDTH/2)/TILE_WIDTH;
    *gridY = (actualY-TILE_WIDTH/2)/TILE_WIDTH;
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