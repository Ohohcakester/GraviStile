#include <tchar.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "gameobjects.h"
#include "GameGlobals.h"
#include "Menu.h"
#include "Textures.h"
#include "Stage.h"
#include "keyboard.h"
#include "globals.h"
#include "main.h"
#include "MenuMain.h"
#include "tests.h"
#include "EditorState.h"
#include "GameStats.h"

int gameStatus = gamestatus_menu;

void rotateRight() {
    Player& player = global::game.player;

    if (!player.canRotate(true)) return;
    int orientation = orientationRotateRight(player.orientation);

    int pivotX, pivotY;
    gridToActual(player.currentPlatform->cx, player.currentPlatform->cy, &pivotX, &pivotY);

    player.rotateTo(orientation);
}

void rotateLeft() {
    Player& player = global::game.player;

    if (!player.canRotate(false)) return;
    int orientation = orientationRotateLeft(player.orientation);

    int pivotX, pivotY;
    gridToActual(player.currentPlatform->cx, player.currentPlatform->cy, &pivotX, &pivotY);

    player.rotateTo(orientation);
}

void restartLevel() {
    if (global::editorState.isActive) {
        editorRestartStage();
    }
    else {
        initialiseGame(global::game.currentStage);
    }
}

void quitGame() {
    if (global::editorState.isActive) {
        tryReturnToEditor();
    }
    else {
        initialiseMenu();
    }
}

void inGameKeyPress(sf::Keyboard::Key keyCode) {
    if (keyCode == sf::Keyboard::Space) global::game.player.jump();
    if (keyCode == sf::Keyboard::A) rotateLeft();
    if (keyCode == sf::Keyboard::D) rotateRight();
    if (keyCode == sf::Keyboard::R) restartLevel();
    if (keyCode == sf::Keyboard::Escape) quitGame();
    if (keyCode == sf::Keyboard::F5) tryReturnToEditor();
    //if (keyCode == sf::Keyboard::T) game.player.currentPlatform->disable(); for debugging.
}

void keyPress(sf::Keyboard::Key keyCode) {
    //std::cout << "Press " << keyCode << std::endl;
    switch (gameStatus) {
    case gamestatus_inGame:
        inGameKeyPress(keyCode);
        break;
    case gamestatus_menu:
        menuKeyPress(keyCode);
        break;
    case gamestatus_editor:
        editorKeyPress(keyCode);
        break;
    }
}

void mouseClick(int x, int y, bool leftClick) {
    //std::cout << (leftClick ? "Left" : "Right") << " click " << x << ", " << y << "\n";
    switch (gameStatus) {
    case gamestatus_inGame:
        break;
    case gamestatus_menu:
        break;
    case gamestatus_editor:
        editorMouseClick(x, y, leftClick);
        break;
    }
}

void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            global::window->close();
            break;
        }
        case sf::Event::KeyPressed: {
            keyPress(event.key.code);
            global::key.updateKey(event.key.code, true);
            break;
        }
        case sf::Event::KeyReleased: {
            global::key.updateKey(event.key.code, false);
            break;
        }
        case sf::Event::MouseButtonReleased: {
            if (event.mouseButton.button == sf::Mouse::Left) {
                mouseClick(event.mouseButton.x, event.mouseButton.y, true);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                mouseClick(event.mouseButton.x, event.mouseButton.y, false);
            }
            break;
        }
    }
}


void initialiseGame(int stage) {
    gameStatus = gamestatus_inGame;
    global::game = GameGlobals();
    GameGlobals& game = global::game;

    game.puzzleComplete = false;
    game.zoom = 0.4f;

    initialiseStage(stage);

    game.assignNewCamera(new Camera(&game.player));
    Grid* grid = &game.grid;
    game.width = grid->sizeX*global::TILE_WIDTH;
    game.height = grid->sizeY*global::TILE_WIDTH;
    Background bg = Background((grid->minX + grid->maxX) / 2, (grid->minY + grid->maxY) / 2);
    game.background = bg;

    game.onStart();
}

void updateGame() {
    GameGlobals& game = global::game;
    Keyboard& key = global::key;

    if (game.puzzleComplete) {
        game.door.endStageTimeout--;
        if (game.door.endStageTimeout <= 0) {
            global::gameStats.stageClear(game.currentStage);
            quitGame();
        }
        return;
    }

    game.updateAllSfx();
    game.background.update(key);
    game.player.update(key);
    game.camera->update(key);
    for (size_t i = 0, n = game.platforms.size(); i < n; ++i) {
        game.platforms[i]->update(key);
    }
    for (size_t i = 0, n = game.lasers.size(); i < n; ++i) {
        game.lasers[i]->update(key);
    }
    for (size_t i = 0, n = game.laserSources.size(); i < n; ++i) {
        game.laserSources[i]->update(key);
    }
    for (size_t i = 0, n = game.laserTargets.size(); i < n; ++i) {
        game.laserTargets[i]->update(key);
    }
    game.door.update(key);

    game.update();
}

void drawGameFrame() {
    GameGlobals& game = global::game;

    game.background.draw();
    game.camera->draw();

    for (size_t i = 0, n = game.platforms.size(); i < n; ++i) {
        game.platforms[i]->draw();
    }
    game.door.draw();
    for (size_t i = 0, n = game.lasers.size(); i < n; ++i) {
        game.lasers[i]->draw();
    }
    for (size_t i = 0, n = game.laserSources.size(); i < n; ++i) {
        game.laserSources[i]->draw();
    }
    for (size_t i = 0, n = game.laserTargets.size(); i < n; ++i) {
        game.laserTargets[i]->draw();
    }
    game.player.draw();


    for (size_t i = 0, n = game.spinConnections.size(); i < n; ++i) {
        game.spinConnections[i]->draw();
    }


    for (size_t i = 0, n = game.switchConnections.size(); i < n; ++i) {
        game.switchConnections[i]->draw();
    }

    for (size_t i = 0, n = game.sfxList.size(); i < n; ++i) {
        if (game.sfxList[i]->isActive) game.sfxList[i]->draw();
    }
}

int main(int argc, TCHAR *argv[]) {
    //test::runTests(); return 0; // Uncomment to run tests.

    sf::RenderWindow w(sf::VideoMode(global::RES_X, global::RES_Y), "GraviStile");
    global::window = &w;
    Textures t;
    global::textures = &t;

    sf::Clock clock;
    
    float frameTime = 1/60.f;
    float dTime = 0;

    initialiseMenu();
    
    while (global::window->isOpen()) {
        sf::Event event;
        while (global::window->pollEvent(event)) {
            processEvent(event);
        }
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        if (dTime > frameTime) {
            dTime -= frameTime;
            
            if (gameStatus == gamestatus_inGame) updateGame();
            else if (gameStatus == gamestatus_editor) updateEditor();
            else if (gameStatus == gamestatus_menu) updateMenu();

            if (dTime < frameTime) {
                // frame skip if lagging
                global::window->clear();

                global::key.update();
                if (gameStatus == gamestatus_inGame) drawGameFrame();
                else if (gameStatus == gamestatus_editor) drawEditorFrame();
                else if (gameStatus == gamestatus_menu) drawMenuFrame();

                else drawGameFrame();
                global::window->display();
            }
        }
    }

    return 0;
}