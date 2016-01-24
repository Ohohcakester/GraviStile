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
#include <set>

using namespace editor;

// Function Prototypes - START
void refreshEditorGameDisplay(bool playGame = false);
void testStage();

GameStage getStage(int stageNo);
int tryAssignId(PlatformTemplate* platformTemplate);
// Function Prototypes - END


void exitEditor() {
    editorState.uninitialise();
    initialiseMenu();
}

void print(std::string message) {
    std::cout << message << "\n";
}

void printCurrentTool() {
    std::string toolName = "???";
    switch (editorState.toolState.state) {
    case tool_none: toolName = "None"; break;
    case tool_move: toolName = "Move"; break;
    case tool_leftTiles: toolName = "Left Tiles"; break;
    case tool_rightTiles: toolName = "Right Tiles"; break;
    case tool_placeDoor: toolName = "Place Door"; break;
    case tool_placePlatform: toolName = "Place Platform"; break;
    case tool_placePlayer: toolName = "Place Player"; break;
    case tool_spinConnection: toolName = "Spin Connection Index"; break;
    case tool_switchConnection: toolName = "Platform Switch Connection Index"; break;
    case tool_quitConfirmation: toolName = "Quit"; break;
    }

    std::cout << "Tool: " << toolName << "\n";
}

void pressKey(int keyNum) {
    SelectionState* selection = &editorState.selectionState;
    ToolState* tools = &editorState.toolState;

    if (tools->state == tool_leftTiles) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->leftTiles = keyNum;
            refreshEditorGameDisplay();

            tools->state = tool_none;
            printCurrentTool();
        }
    }

    if (tools->state == tool_rightTiles) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->rightTiles = keyNum;
            refreshEditorGameDisplay();

            tools->state = tool_none;
            printCurrentTool();
        }
    }

    if (tools->state == tool_spinConnection) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->spinConnectionIndex = keyNum;
            refreshEditorGameDisplay();

            tools->state = tool_none;
            printCurrentTool();
        }
    }

    if (tools->state == tool_switchConnection) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->platformSwitchConnectionIndex = keyNum;
            refreshEditorGameDisplay();

            tools->state = tool_none;
            printCurrentTool();
        }
        else if (selection->type == selection_switch) {
            LaserTargetTemplate* lTarget = selection->selectedSwitch;
            lTarget->switchConnectionIndex = keyNum;
            refreshEditorGameDisplay();

            tools->state = tool_none;
            printCurrentTool();
        }
    }
}


void editorKeyPress(sf::Keyboard::Key keyCode) {
    SelectionState* selection = &editorState.selectionState;
    ToolState* tools = &editorState.toolState;
    EditableLevelTemplate* templ = &editorState.levelTemplate;

    if (keyCode == sf::Keyboard::A) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->orientation = orientationRotateLeft(plat->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_player) {
            PlayerTemplate* play = &templ->player;
            play->orientation = orientationRotateLeft(play->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_door) {
            DoorTemplate* door = &templ->door;
            door->orientation = orientationRotateLeft(door->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_laserSource) {
            LaserSourceTemplate* laser = selection->selectedLaserSource;
            laser->orientation = orientationRotateLeft(laser->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_switch) {
            LaserTargetTemplate* swt = selection->selectedSwitch;
            swt->orientation = orientationRotateLeft(swt->orientation);
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::D) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->orientation = orientationRotateRight(plat->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_player) {
            PlayerTemplate* play = &templ->player;
            play->orientation = orientationRotateRight(play->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_door) {
            DoorTemplate* door = &templ->door;
            door->orientation = orientationRotateRight(door->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_laserSource) {
            LaserSourceTemplate* laser = selection->selectedLaserSource;
            laser->orientation = orientationRotateRight(laser->orientation);
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_switch) {
            LaserTargetTemplate* swt = selection->selectedSwitch;
            swt->orientation = orientationRotateRight(swt->orientation);
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::R) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->rotatable = !plat->rotatable;
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::E) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            plat->isDisabled = !plat->isDisabled;
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::Delete) {
        if (selection->type == selection_platform) {
            templ->remove(selection->selectedPlatform);
            selection->clear();
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_laserSource) {
            templ->remove(selection->selectedLaserSource);
            selection->clear();
            refreshEditorGameDisplay();
        }
        else if (selection->type == selection_switch) {
            templ->remove(selection->selectedSwitch);
            selection->clear();
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::G) {
        if (tools->state == tool_leftTiles) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_leftTiles;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::H) {
        if (tools->state == tool_rightTiles) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_rightTiles;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::Z) {
        templ->zoom = game.camera->zoom;
        std::cout << "Zoom set to current value of " << templ->zoom << "\n";
    }

    if (keyCode == sf::Keyboard::S) {
        if (tools->state == tool_spinConnection) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_spinConnection;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::Q) {
        if (tools->state == tool_switchConnection) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_switchConnection;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::Dash) pressKey(-1);
    if (keyCode == sf::Keyboard::Num0) pressKey(0);
    if (keyCode == sf::Keyboard::Num1) pressKey(1);
    if (keyCode == sf::Keyboard::Num2) pressKey(2);
    if (keyCode == sf::Keyboard::Num3) pressKey(3);
    if (keyCode == sf::Keyboard::Num4) pressKey(4);
    if (keyCode == sf::Keyboard::Num5) pressKey(5);
    if (keyCode == sf::Keyboard::Num6) pressKey(6);
    if (keyCode == sf::Keyboard::Num7) pressKey(7);
    if (keyCode == sf::Keyboard::Num8) pressKey(8);
    if (keyCode == sf::Keyboard::Num9) pressKey(9);

    if (keyCode == sf::Keyboard::LShift) {
        if (tools->state == tool_move) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_move;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::Left) {
        if (tools->state == tool_move) {
            if (selection->type == selection_platform) {
                PlatformTemplate* plat = selection->selectedPlatform;
                plat->pivotX--;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_player) {
                PlayerTemplate* play = &templ->player;
                play->x--;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_door) {
                DoorTemplate* door = &templ->door;
                door->x--;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_laserSource) {
                LaserSourceTemplate* laser = selection->selectedLaserSource;
                laser->position--;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_switch) {
                LaserTargetTemplate* swt = selection->selectedSwitch;
                swt->position--;
                refreshEditorGameDisplay();
            }
        }
    }

    if (keyCode == sf::Keyboard::Right) {
        if (tools->state == tool_move) {
            if (selection->type == selection_platform) {
                PlatformTemplate* plat = selection->selectedPlatform;
                plat->pivotX++;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_player) {
                PlayerTemplate* play = &templ->player;
                play->x++;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_door) {
                DoorTemplate* door = &templ->door;
                door->x++;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_laserSource) {
                LaserSourceTemplate* laser = selection->selectedLaserSource;
                laser->position++;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_switch) {
                LaserTargetTemplate* swt = selection->selectedSwitch;
                swt->position++;
                refreshEditorGameDisplay();
            }
        }
    }

    if (keyCode == sf::Keyboard::Up) {
        if (tools->state == tool_move) {
            if (selection->type == selection_platform) {
                PlatformTemplate* plat = selection->selectedPlatform;
                plat->pivotY--;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_player) {
                PlayerTemplate* play = &templ->player;
                play->y--;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_door) {
                DoorTemplate* door = &templ->door;
                door->y--;
                refreshEditorGameDisplay();
            }
        }
    }

    if (keyCode == sf::Keyboard::Down) {
        if (tools->state == tool_move) {
            if (selection->type == selection_platform) {
                PlatformTemplate* plat = selection->selectedPlatform;
                plat->pivotY++;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_player) {
                PlayerTemplate* play = &templ->player;
                play->y++;
                refreshEditorGameDisplay();
            }
            else if (selection->type == selection_door) {
                DoorTemplate* door = &templ->door;
                door->y++;
                refreshEditorGameDisplay();
            }
        }
    }

    if (keyCode == sf::Keyboard::P) {
        if (tools->state == tool_placePlatform) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_placePlatform;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::O) {
        if (tools->state == tool_placeDoor) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_placeDoor;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::I) {
        if (tools->state == tool_placePlayer) {
            tools->state = tool_none;
        }
        else {
            tools->state = tool_placePlayer;
        }
        printCurrentTool();
    }

    if (keyCode == sf::Keyboard::L) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            int id = tryAssignId(plat);
            templ->laserSources.push_back(LaserSourceTemplate(0, dir_up, id));
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::K) {
        if (selection->type == selection_platform) {
            PlatformTemplate* plat = selection->selectedPlatform;
            int id = tryAssignId(plat);
            templ->laserTargets.push_back(LaserTargetTemplate(0, dir_up, id, -1));
            refreshEditorGameDisplay();
        }
    }

    if (keyCode == sf::Keyboard::Y && tools->state == tool_quitConfirmation) {
        exitEditor();
        std::cout << "Exiting Editor\n";
    }

    if (keyCode == sf::Keyboard::N && tools->state == tool_quitConfirmation) {
        tools->state = tool_none;
        printCurrentTool();
    }   

    if (keyCode == sf::Keyboard::F5) testStage();
    if (keyCode == sf::Keyboard::Escape) {
        tools->state = tool_quitConfirmation;
        std::cout << "Are you sure you want to quit? Y/N\n";
        printCurrentTool();
    }
    if (keyCode == sf::Keyboard::Return) {
        editorState.levelTemplate.generateCode();
        print("Code Generated");
    }
}


void trySelect(double x, double y) {
    SelectionState* selection = &editorState.selectionState;
    EditableLevelTemplate* templ = &editorState.levelTemplate;

    for (size_t i = 0, n = game.laserSources.size(); i < n; ++i) {
        LaserSource* l = game.laserSources[i];
        if (l->isWithinClickHitbox(x, y)) {
            selection->selectLaserSource(&templ->laserSources[i]);
            std::cout << "Select Laser Source " << i << "\n";
            return;
        }
    }

    for (size_t i = 0, n = game.laserTargets.size(); i < n; ++i) {
        LaserTarget* l = game.laserTargets[i];
        if (l->isWithinClickHitbox(x, y)) {
            selection->selectSwitch(&templ->laserTargets[i]);
            std::cout << "Select Laser Target " << i << "\n";
            return;
        }
    }

    for (size_t i = 0, n = game.platforms.size(); i < n; ++i) {
        Platform* p = game.platforms[i];
        if (p->isWithinClickHitbox(x, y)) {
            selection->selectPlatform(&templ->platforms[i]);
            std::cout << "Select Platform " << i << "\n";
            return;
        }
    }

    if (game.player.isWithinClickHitbox(x, y)) {
        selection->selectPlayer();
        std::cout << "Select Player\n";
        return;
    }

    if (game.door.isWithinClickHitbox(x, y)) {
        selection->selectDoor();
        std::cout << "Select Door\n";
        return;
    }


    selection->clear();
    std::cout << "Deselect\n";
}

int tryAssignId(PlatformTemplate* platform) {
    if (platform->id != -1) return platform->id;

    EditableLevelTemplate* templ = &editorState.levelTemplate;

    std::set<int> takenIds;
    for (size_t i = 0, n = templ->platforms.size(); i < n; ++i) {
        PlatformTemplate* p = &templ->platforms[i];
        if (p->id != -1) takenIds.insert(p->id);
    }
    int newId = 0;
    while (takenIds.find(newId) != takenIds.end()) {
        newId++;
    }

    platform->id = newId;
    return platform->id;
}

void placePlatform(int x, int y) {
    int cx, cy;
    actualToGrid(x, y, &cx, &cy);
    editorState.levelTemplate.platforms.push_back(PlatformTemplate(cx, cy, 1, 1, true, dir_up));

    refreshEditorGameDisplay();
}

void placePlayer(int x, int y) {
    int cx, cy;
    actualToGrid(x, y, &cx, &cy);

    editorState.levelTemplate.player.x = cx;
    editorState.levelTemplate.player.y = cy;

    refreshEditorGameDisplay();
}

void placeDoor(int x, int y) {
    int cx, cy;
    actualToGrid(x, y, &cx, &cy);

    editorState.levelTemplate.door.x = cx;
    editorState.levelTemplate.door.y = cy;

    refreshEditorGameDisplay();
}

void editorMouseClick(int x, int y, bool leftClick) {
    float absX = x, absY = y;
    game.camera->toAbs(&absX, &absY);

    ToolState* tools = &editorState.toolState;

    //std::cout << "Absolute: " << absX << ", " << absY << "\n";
    if (leftClick) {
        if (tools->state == tool_placePlatform) {
            placePlatform(absX, absY);
            tools->state = tool_none;
        }
        if (tools->state == tool_placePlayer) {
            placePlayer(absX, absY);
            tools->state = tool_none;
        }
        if (tools->state == tool_placeDoor) {
            placeDoor(absX, absY);
            tools->state = tool_none;
        }
        else {
            trySelect(absX, absY);
        }
    }
}

void refreshEditorGameDisplay(bool playMode) {
    game = GameGlobals();
    game.puzzleComplete = false;

    initialiseFromStageObject(editorState.levelTemplate.generateStage());

    if (playMode) {
        game.assignNewCamera(new Camera(&game.player));
    }
    else {
        game.assignNewCamera(new EditorCamera(editorState.camX, editorState.camY, editorState.camZoom));
    }
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
    editorState.levelTemplate = EditableLevelTemplate(getStage(24));

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


void tryReturnToEditor() {
    if (!editorState.isActive) return;

    gameStatus = gamestatus_editor;
    refreshEditorGameDisplay();
}

void testStage() {
    gameStatus = gamestatus_inGame;
    refreshEditorGameDisplay(true);
}

void editorRestartStage() {
    gameStatus = gamestatus_inGame;
    refreshEditorGameDisplay(true);
}