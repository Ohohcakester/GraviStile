#include <SFML/Graphics.hpp>
#include <iostream>
#include "keyboard.h"
#include "gameobjects.h"
#include "globals.h"

bool inMenu = true;

void initialiseGame(int stage);

void rotateRight() {
    if (!game.player.canRotate(false)) return;
    int orientation = game.player.orientation;
    orientation += 1;
    if (orientation >= 4) orientation = 0;

    int pivotX, pivotY;
    gridToActual(game.player.currentPlatform->cx, game.player.currentPlatform->cy, &pivotX, &pivotY);

    game.camera.rotateTo(orientation, pivotX, pivotY);
    game.player.rotateTo(orientation);
}

void rotateLeft() {
    if (!game.player.canRotate(true)) return;
    int orientation = game.player.orientation;
    orientation -= 1;
    if (orientation < 0) orientation = 3;

    int pivotX, pivotY;
    gridToActual(game.player.currentPlatform->cx, game.player.currentPlatform->cy, &pivotX, &pivotY);

    game.camera.rotateTo(orientation, pivotX, pivotY);
    game.player.rotateTo(orientation);
}

void keyPress(sf::Keyboard::Key keyCode) {
    std::cout << "Press " << keyCode << std::endl;
    if (inMenu) {
        if (keyCode == sf::Keyboard::Space) initialiseGame(menu.selection);
        if (keyCode == sf::Keyboard::Left) menu.previous();
        if (keyCode == sf::Keyboard::Right) menu.next();
        if (keyCode == sf::Keyboard::Up) {for (int i=0;i<menu.cols;++i) menu.previous();}
        if (keyCode == sf::Keyboard::Down) {for (int i=0;i<menu.cols;++i) menu.next();}
    } else {
        if (keyCode == sf::Keyboard::Space) game.player.jump();
        if (keyCode == sf::Keyboard::A) rotateLeft();
        if (keyCode == sf::Keyboard::D) rotateRight();
        if (keyCode == sf::Keyboard::Escape) initialiseGame(game.currentStage);
    }
}


void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window.close();
            break;
        }
        case sf::Event::KeyPressed: {
            keyPress(event.key.code);
        }
    }
}

void initialiseLevel0() {
    int nPlats = 5;
    Platform plats[nPlats] = {
        Platform(8, 7, 3, 4, true, dir_up),
        Platform(1, 6, 1, 2, false, dir_down),
        Platform(8, 10, 5, 6, true, dir_up),
        Platform(16, 11, 1, 6, true, dir_right),
        Platform(1, 10, 2, 2, true, dir_right)
    };
    
    game.nTilesX = 10;
    game.nTilesY = 10;
    game.player = Player();
    gridToActual(8, 3, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
}


void initialiseLevel1() {
    int nPlats = 10;
    Platform plats[nPlats] = {
        Platform(2, 2, 1, 1, true, dir_up),
        Platform(4, 1, 0, 0, true, dir_right),
        Platform(3, 5, 2, 2, true, dir_up),
        Platform(6, 6, 1, 1, true, dir_right),
        Platform(5, 8, 1, 1, true, dir_down),
        Platform(8, 7, 1, 1, true, dir_up),
        Platform(3, 9, 0, 0, true, dir_left),
        Platform(5, 11, 1, 1, true, dir_right),
        Platform(9, 9, 2, 2, false, dir_up),
        Platform(7, 0, 1, 1, true, dir_down),
    };
    
    game.nTilesX = 12;
    game.nTilesY = 12;
    game.player = Player();
    gridToActual(3, 4, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.7;
    Door door = Door(6, 12, dir_right);
    game.door = door;
}

void initialiseGame(int stage) {
    inMenu = false;
    game = GameGlobals();
    game.puzzleComplete = false;
    game.zoom = 0.4;

    game.currentStage = stage;
    switch(stage) {
        case 0: initialiseLevel0(); break;
        case 1: initialiseLevel1(); break;
    }

    game.camera = Camera(&game.player);
    game.width = game.nTilesX*TILE_WIDTH;
    game.height = game.nTilesY*TILE_WIDTH;
    Background bg = Background();
    game.background = bg;
}

void initialiseMenu() {
    inMenu = true;
}

void updateGame() {
    if (game.puzzleComplete) return;

    game.key.update();
    game.background.update(game.key);
    game.player.update(game.key);
    game.camera.update(game.key);
    for (int i=0;i<game.platforms.size();++i) {
        game.platforms[i].update(game.key);
    }
    game.door.update(game.key);
}

void updateMenu() {

}

void drawGameFrame() {
    game.background.draw();
    game.camera.draw();
    for (int i=0;i<game.platforms.size();++i) {
        game.platforms[i].draw();
    }
    game.door.draw();
    game.player.draw();
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
            shape3.setFillColor(sf::Color::Yellow);
            shape3.setSize(sf::Vector2f(itemGlowWidth,itemGlowWidth));
            shape3.setPosition(itemSpacing*(col+0.5f)-halfItemGlowWidth, itemSpacing*(row+0.5f)-halfItemGlowWidth);
            window.draw(shape3);
        }

        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Magenta);
        shape.setSize(sf::Vector2f(itemOutlineWidth,itemOutlineWidth));
        shape.setPosition(itemSpacing*(col+0.5f)-halfItemOutlineWidth, itemSpacing*(row+0.5f)-halfItemOutlineWidth);
        window.draw(shape);

        sf::RectangleShape shape2;
        shape.setFillColor(sf::Color::Blue);
        shape2.setSize(sf::Vector2f(itemWidth,itemWidth));
        shape2.setPosition(itemSpacing*(col+0.5f)-halfItemWidth, itemSpacing*(row+0.5f)-halfItemWidth);
        window.draw(shape2);
    }
}

int main() {
    sf::Clock clock;

    float frameTime = 1/60.f;
    float dTime = 0;

    initialiseMenu();
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        if (dTime > frameTime) {
            dTime -= frameTime;
            
            if (inMenu) updateMenu();
            else updateGame();

            if (dTime < frameTime) {
                // frame skip if lagging
                window.clear();
                if (inMenu) drawMenuFrame();
                else drawGameFrame();
                window.display();
            }
        }
    }

    return 0;
}