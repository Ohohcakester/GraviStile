#ifndef GLOBALS_H
#define GLOBALS_H
#include "gameobjects.h"
#include "keyboard.h"

class GameGlobals {
public:
    Player player;
    Keyboard key;
};

extern GameGlobals game;

extern int RES_X;
extern int RES_Y;
extern int WIDTH;
extern int HEIGHT;

extern sf::RenderWindow window;







#endif