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
#include "tests.h";
#include "EditorState.h"

int gameStatus = gamestatus_menu;

void rotateRight() {
    if (!game.player.canRotate(true)) return;
    int orientation = orientationRotateRight(game.player.orientation);

    int pivotX, pivotY;
    gridToActual(game.player.currentPlatform->cx, game.player.currentPlatform->cy, &pivotX, &pivotY);

    game.player.rotateTo(orientation);
}

void rotateLeft() {
    if (!game.player.canRotate(false)) return;
    int orientation = orientationRotateLeft(game.player.orientation);

    int pivotX, pivotY;
    gridToActual(game.player.currentPlatform->cx, game.player.currentPlatform->cy, &pivotX, &pivotY);

    game.player.rotateTo(orientation);
}

void restartLevel() {
    if (editorState.isActive) {
        editorRestartStage();
    }
    else {
        initialiseGame(game.currentStage);
    }
}

void quitGame() {
    if (editorState.isActive) {
        tryReturnToEditor();
    }
    else {
        initialiseMenu();
    }
}

void inGameKeyPress(sf::Keyboard::Key keyCode) {
    if (keyCode == sf::Keyboard::Space) game.player.jump();
    if (keyCode == sf::Keyboard::A) rotateLeft();
    if (keyCode == sf::Keyboard::D) rotateRight();
    if (keyCode == sf::Keyboard::R) restartLevel();
    if (keyCode == sf::Keyboard::Escape) quitGame();
    if (keyCode == sf::Keyboard::F5) tryReturnToEditor();
    //if (keyCode == sf::Keyboard::T) game.player.currentPlatform->disable(); for debugging.
}

void menuKeyPress(sf::Keyboard::Key keyCode) {
    if (keyCode == sf::Keyboard::Return) initialiseGame(menu.selection + 1);
    if (keyCode == sf::Keyboard::Space) initialiseGame(menu.selection + 1);
    if (keyCode == sf::Keyboard::Left) menu.previous();
    if (keyCode == sf::Keyboard::Right) menu.next();
    if (keyCode == sf::Keyboard::Up) menu.up();
    if (keyCode == sf::Keyboard::Down) menu.down();

    if (keyCode == sf::Keyboard::Num1) initialiseGame(100);
    if (keyCode == sf::Keyboard::Num2) initialiseGame(101);
    if (keyCode == sf::Keyboard::Num3) initialiseGame(102);

    if (keyCode == sf::Keyboard::BackSpace) initialiseEditor();
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
            window->close();
            break;
        }
        case sf::Event::KeyPressed: {
            keyPress(event.key.code);
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
    game = GameGlobals();
    game.puzzleComplete = false;
    game.zoom = 0.4f;

    initialiseStage(stage);

    game.assignNewCamera(new Camera(&game.player));
    Grid* grid = &game.grid;
    game.width = grid->sizeX*TILE_WIDTH;
    game.height = grid->sizeY*TILE_WIDTH;
    Background bg = Background((grid->minX + grid->maxX) / 2, (grid->minY + grid->maxY) / 2);
    game.background = bg;

    game.onStart();
}

void initialiseMenu() {
    gameStatus = gamestatus_menu;
}

void updateGame() {
    if (game.puzzleComplete) {
        game.door.endStageTimeout--;
        if (game.door.endStageTimeout <= 0) {
            quitGame();
        }
        return;
    }

    game.updateAllSfx();
    game.key.update();
    game.background.update(game.key);
    game.player.update(game.key);
    game.camera->update(game.key);
    for (size_t i = 0; i<game.platforms.size(); ++i) {
        game.platforms[i]->update(game.key);
    }
    game.door.update(game.key);

    game.update();
}

void updateMenu() {

}

void drawGameFrame() {
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

void drawMenuFrame() {
    float itemSpacing = RES_X / menu.cols;
    float itemGlowWidth = itemSpacing*0.7f;
    float halfItemGlowWidth = itemGlowWidth/2;

    float itemOutlineWidth = itemSpacing*0.6f;
    float halfItemOutlineWidth = itemOutlineWidth/2;

    float itemWidth = itemSpacing*0.5f;
    float halfItemWidth = itemWidth/2;

    for (int i=0;i<menu.nItems;++i) {
        int col = i%menu.cols;
        int row = i/menu.cols;

        if (i == menu.selection) {
            sf::RectangleShape shape3;
            shape3.setFillColor(sf::Color::White);
            shape3.setSize(sf::Vector2f(itemGlowWidth,itemGlowWidth));
            shape3.setPosition(itemSpacing*(col+0.5f)-halfItemGlowWidth, itemSpacing*(row+0.5f)-halfItemGlowWidth);
            window->draw(shape3);
        }

        sf::RectangleShape shape;
        shape.setFillColor(textures->platformColor);
        shape.setSize(sf::Vector2f(itemOutlineWidth,itemOutlineWidth));
        shape.setPosition(itemSpacing*(col+0.5f)-halfItemOutlineWidth, itemSpacing*(row+0.5f)-halfItemOutlineWidth);
        window->draw(shape);

        sf::RectangleShape shape2;
        shape2.setFillColor(textures->platformSurfaceColor);
        shape2.setSize(sf::Vector2f(itemWidth,itemWidth));
        shape2.setPosition(itemSpacing*(col+0.5f)-halfItemWidth, itemSpacing*(row+0.5f)-halfItemWidth);
        window->draw(shape2);
        
        std::ostringstream ss;
        ss << i + 1;
        sf::Text numbering;
        numbering.setFont(textures->comicsans);
        numbering.setString(ss.str());
        numbering.setCharacterSize(40);
        numbering.setColor(textures->platformColor);
        sf::FloatRect textRect = numbering.getLocalBounds();
        numbering.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        numbering.setPosition(itemSpacing*(col + 0.5f), itemSpacing*(row + 0.5f));
        window->draw(numbering);
    }
}

int main(int argc, TCHAR *argv[]) {
    //test::runTests(); return 0; // Uncomment to run tests.

    sf::RenderWindow w(sf::VideoMode(RES_X, RES_Y), "GraviStile");
    window = &w;
    Textures t;
    textures = &t;

    sf::Clock clock;
    
    float frameTime = 1/60.f;
    float dTime = 0;

    initialiseMenu();
    
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
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
                window->clear();

                if (gameStatus == gamestatus_inGame) drawGameFrame();
                else if (gameStatus == gamestatus_editor) drawEditorFrame();
                else if (gameStatus == gamestatus_menu) drawMenuFrame();

                else drawGameFrame();
                window->display();
            }
        }
    }

    return 0;
}