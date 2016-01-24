#ifndef MAIN_H_
#define MAIN_H_

#include "EditorMain.h"

enum GameStatus {
    gamestatus_inGame,
    gamestatus_menu,
    gamestatus_editor,
};

extern int gameStatus;

void initialiseGame(int stage);
void initialiseMenu();

void drawGameFrame();

#endif