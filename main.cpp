#include <SFML/Graphics.hpp>
#include <iostream>
#include "keyboard.h"
#include "gameobjects.h"
#include "globals.h"

bool inMenu = true;

void initialiseGame(int stage);
void initialiseMenu();

void rotateRight() {
    if (!game.player.canRotate(true)) return;
    int orientation = game.player.orientation;
    orientation += 1;
    if (orientation >= 4) orientation = 0;

    int pivotX, pivotY;
    gridToActual(game.player.currentPlatform->cx, game.player.currentPlatform->cy, &pivotX, &pivotY);

    game.camera.rotateTo(orientation, pivotX, pivotY);
    game.player.rotateTo(orientation);
}

void rotateLeft() {
    if (!game.player.canRotate(false)) return;
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
        if (keyCode == sf::Keyboard::R) initialiseGame(game.currentStage);
        if (keyCode == sf::Keyboard::Escape) initialiseMenu();
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



/* ======================== *
 *     REGION - LEVELS      *
 * ======================== */


void initialiseLevel0() {
    int nPlats = 2;
    Platform plats[nPlats] = {
        Platform(1, 1, 1, 1, false, dir_up),
        Platform(5, 1, 1, 1, false, dir_up),
    };
    
    game.nTilesX = 7;
    game.nTilesY = 3;
    game.player = Player();
    gridToActual(0, 0, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 1.2;
    Door door = Door(6, 0, dir_up);
    game.door = door;
}


void initialiseLevel1() {
    int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(1, 2, 1, 2, false, dir_up),
        Platform(2, 4, 0, 1, false, dir_up),
        Platform(5, 1, 1, 1, true, dir_up),
        Platform(5, 4, 1, 1, true, dir_left),
    };
    
    game.nTilesX = 7;
    game.nTilesY = 6;
    game.player = Player();
    gridToActual(0, 0, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 1.0;
    Door door = Door(2, 3, dir_up);
    game.door = door;
}


void initialiseLevel2() {
    int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(1, 2, 1, 2, true, dir_up),
        Platform(4, 2, 1, 2, true, dir_left),
        Platform(5, 0, 0, 0, true, dir_up),
        Platform(7, 2, 1, 0, true, dir_down),
    };
    
    game.nTilesX = 9;
    game.nTilesY = 4;
    game.player = Player();
    gridToActual(0, 1, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.9;
    Door door = Door(7, 1, dir_up);
    game.door = door;
}


void initialiseLevel3() {
    int nPlats = 6;
    Platform plats[nPlats] = {
        Platform(2, 0, 1, 1, false, dir_up),
        Platform(2, 2, 1, 1, true, dir_up),
        Platform(0, 4, 1, 1, true, dir_right),
        Platform(4, 4, 1, 1, true, dir_left),
        Platform(0, 0, 0, 0, true, dir_down),
        Platform(4, 0, 0, 0, true, dir_down),
    };
    
    game.nTilesX = 5;
    game.nTilesY = 6;
    game.player = Player();
    gridToActual(1, 1, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 1.0;
    Door door = Door(2, 1, dir_down);
    game.door = door;
}



void initialiseLevel4() {
    int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(1, 3, 0, 2, true, dir_up),
        Platform(2, 6, 1, 0, true, dir_up),
        Platform(2, 8, 0, 0, true, dir_right),
        Platform(4, 1, 1, 0, true, dir_left),
    };
    
    game.nTilesX = 5;
    game.nTilesY = 9;
    game.player = Player();
    gridToActual(2, 2, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.8;
    Door door = Door(3, 0, dir_up);
    game.door = door;
}


void initialiseLevel5() {
    int nPlats = 7;
    Platform plats[nPlats] = {
        Platform(3, 1, 0, 0, true, dir_right),
        Platform(3, 4, 0, 0, true, dir_up),
        Platform(4, 3, 0, 0, true, dir_down),
        Platform(5, 2, 0, 0, true, dir_up),
        Platform(5, 5, 0, 0, true, dir_down),
        Platform(2, 6, 0, 2, true, dir_right),
        Platform(1, 5, 0, 0, true, dir_right),
    };
    
    game.nTilesX = 8;
    game.nTilesY = 9;
    game.player = Player();
    gridToActual(3, 0, &game.player.x, &game.player.y);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.9;
    Door door = Door(3, 7, dir_down);
    game.door = door;
}





void initialiseLevel13() {
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














/* ======================== *
 *   REGION - LEVELS - END  *
 * ======================== */







void initialiseGame(int stage) {
    inMenu = false;
    game = GameGlobals();
    game.puzzleComplete = false;
    game.zoom = 0.4;

    game.currentStage = stage;
    switch(stage) {
        case 0: initialiseLevel0(); break;
        case 1: initialiseLevel1(); break;
        case 2: initialiseLevel2(); break;
        case 3: initialiseLevel3(); break;
        case 4: initialiseLevel4(); break;
        case 5: initialiseLevel5(); break;
        case 13: initialiseLevel13(); break;
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
    if (game.puzzleComplete) {
        game.door.endStageTimeout--;
        if (game.door.endStageTimeout <= 0) {
            initialiseMenu();
        }
        return;
    }

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