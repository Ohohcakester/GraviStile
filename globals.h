#ifndef GLOBALS_H
#define GLOBALS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "gameobjects.h"
#include "keyboard.h"

class GameGlobals {
public:
    int currentStage;

    Player player;
    Keyboard key;
    Door door;
    Camera camera;
    Background background;

    int nTilesX;
    int nTilesY;
    int width;
    int height;

    float zoom;

    bool puzzleComplete;
    
    std::vector<Platform> platforms;

    GameGlobals();
};

class Menu {
public:
    int cols;
    int nItems;
    int selection;

    Menu();
    void next();
    void previous();
};

class Textures {
public:
    sf::Texture background;
    sf::Texture player;
    sf::Texture door;
    sf::Texture pivot;
    sf::Color platformColor;
    sf::Color platformSurfaceColor;

    Textures();
};

extern GameGlobals game;
extern Textures* textures;
extern Menu menu;

extern int RES_X;
extern int RES_Y;
extern int TILE_WIDTH;

extern sf::RenderWindow* window;

void gridToActual(int gridX, int gridY, int* actualX, int* actualY);
void actualToGrid(int actualX, int actualY, int* gridX, int* gridY);

void rotateAboutPivotActual(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y);
void rotateAboutPivotGrid(int oldOrientation, int newOrientation, int gridcx, int gridcy, int* x, int* y);




#endif