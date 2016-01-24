#include <iostream>
#include "EditorMain.h"
#include "EditorState.h"
#include "SelectionState.h"
#include "EditableLevelTemplate.h"
#include "ToolState.h"
#include "GameGlobals.h"
#include "globals.h"
#include "main.h"
#include "EditorCamera.h"
#include "Stage.h"
#include "gamemath.h"
#include "Orientations.h"

using namespace editor;
void refreshEditorGameDisplay();

GameStage getStage(int stageNo);

void exitEditor() {
    editorState.uninitialise();
    initialiseMenu();
}

void print(std::string message) {
    std::cout << message << "\n";
}


void editorKeyPress(sf::Keyboard::Key keyCode) {
    SelectionState* selection = &editorState.selectionState;
    ToolState* tools = &editorState.toolState;

    if (keyCode == sf::Keyboard::A) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->orientation = orientationRotateLeft(plat->orientation);
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::D) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->orientation = orientationRotateRight(plat->orientation);
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::LShift) {
        if (tools->state == tool_movePlatform) {
            print("Tool: None");
            tools->state = tool_none;
        }
        else {
            print("Tool: Move");
            tools->state = tool_movePlatform;
        }
    }

    if (keyCode == sf::Keyboard::Left) {
        if (selection->type == selection_platform && tools->state == tool_movePlatform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->pivotX--;
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::Right) {
        if (selection->type == selection_platform && tools->state == tool_movePlatform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->pivotX++;
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::Up) {
        if (selection->type == selection_platform && tools->state == tool_movePlatform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->pivotY--;
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::Down) {
        if (selection->type == selection_platform && tools->state == tool_movePlatform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->pivotY++;
            refreshEditorGameDisplay();
        }
    }


    if (keyCode == sf::Keyboard::Escape) exitEditor();
}


void trySelect(double x, double y) {
    SelectionState* selection = &editorState.selectionState;
    EditableLevelTemplate* templ = &editorState.levelTemplate;

    for (size_t i = 0, n = game.platforms.size(); i < n; ++i) {
        Platform* p = game.platforms[i];
        if (isWithinRect(x, y, p->x1, p->y1, p->x2, p->y2)) {
            selection->selectPlatform(&templ->platforms[i]);
            return;
        }
    }
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

    game.assignNewCamera(new EditorCamera(editorState.camX, editorState.camY, editorState.camZoom));
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

    editorState.camX = game.camera->px;
    editorState.camY = game.camera->py;
    editorState.camZoom = game.camera->zoom;
}