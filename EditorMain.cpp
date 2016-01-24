#include <iostream>
#include "EditorMain.h"
#include "EditorState.h"
#include "GameGlobals.h"
#include "globals.h"
#include "main.h"
#include "EditorCamera.h"
#include "Stage.h"

void exitEditor() {
    editorState.uninitialise();
    initialiseMenu();
}

void editorKeyPress(sf::Keyboard::Key keyCode) {
    



    if (keyCode == sf::Keyboard::Escape) exitEditor();
}

void trySelect(double x, double y) {
    
}

void editorMouseClick(int x, int y, bool leftClick) {
    float absX = x, absY = y;
    game.camera->toAbs(&absX, &absY);

    //std::cout << "Absolute: " << absX << ", " << absY << "\n";
    if (leftClick) {
        trySelect(absX, absY);
    }
}


void initialiseEditor() {
    gameStatus = gamestatus_editor;
    editorState.initialise();

    game = GameGlobals();
    game.puzzleComplete = false;
    game.zoom = 0.9f;

    initialiseStage(102);

    game.assignNewCamera(new EditorCamera());
    Grid* grid = &game.grid;
    game.width = grid->sizeX*TILE_WIDTH;
    game.height = grid->sizeY*TILE_WIDTH;
    Background bg = Background((grid->minX + grid->maxX) / 2, (grid->minY + grid->maxY) / 2);
    game.background = bg;

    game.onStart();
}


void drawEditorFrame() {
    drawGameFrame();
}


void updateEditor() {
    game.key.update();

    game.camera->update(game.key);
}