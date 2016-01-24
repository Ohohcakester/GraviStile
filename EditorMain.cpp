#include <iostream>
#include "EditorMain.h"
#include "EditorState.h"
#include "EditableLevelTemplate.h"
#include "GameGlobals.h"
#include "globals.h"
#include "main.h"
#include "EditorCamera.h"
#include "Stage.h"

using namespace editor;

GameStage getStage(int stageNo);

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

void refreshEditorGameDisplay() {
    game = GameGlobals();
    game.puzzleComplete = false;

    initialiseFromStageObject(editorState.levelTemplate.generateStage());

    game.assignNewCamera(new EditorCamera());
    Grid* grid = &game.grid;
    game.width = grid->sizeX*TILE_WIDTH;
    game.height = grid->sizeY*TILE_WIDTH;
    Background bg = Background((grid->minX + grid->maxX) / 2, (grid->minY + grid->maxY) / 2);
    game.background = bg;

    game.onStart();
}

void initialiseEditor() {
    gameStatus = gamestatus_editor;
    editorState.initialise();
    editorState.levelTemplate = EditableLevelTemplate(getStage(102));

    refreshEditorGameDisplay();
}


void drawEditorFrame() {
    drawGameFrame();
}


void updateEditor() {
    game.key.update();

    game.camera->update(game.key);
}