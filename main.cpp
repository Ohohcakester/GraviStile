#include <tchar.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
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
    //std::cout << "Press " << keyCode << std::endl;
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
            window->close();
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
    const int nPlats = 2;
    Platform plats[nPlats] = {
        Platform(1, 1, 1, 1, false, dir_up),
        Platform(5, 1, 1, 1, false, dir_up),
    };
    
    game.nTilesX = 7;
    game.nTilesY = 3;
    game.player = Player(0, 0);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 1.2;
    Door door = Door(6, 0, dir_up);
    game.door = door;
}


void initialiseLevel1() {
    const int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(1, 2, 1, 2, false, dir_up),
        Platform(2, 4, 0, 1, false, dir_up),
        Platform(5, 1, 1, 1, true, dir_up),
        Platform(5, 4, 1, 1, true, dir_left),
    };
    
    game.nTilesX = 7;
    game.nTilesY = 6;
    game.player = Player(0, 0);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 1.0;
    Door door = Door(2, 3, dir_up);
    game.door = door;
}


void initialiseLevel2() {
    const int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(1, 2, 1, 2, true, dir_up),
        Platform(4, 2, 1, 2, true, dir_left),
        Platform(5, 0, 0, 0, true, dir_up),
        Platform(7, 2, 1, 0, true, dir_down),
    };
    
    game.nTilesX = 9;
    game.nTilesY = 4;
    game.player = Player(0, 1);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.9;
    Door door = Door(7, 1, dir_up);
    game.door = door;
}


void initialiseLevel3() {
    const int nPlats = 6;
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
    game.player = Player(1, 1);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 1.0;
    Door door = Door(2, 1, dir_down);
    game.door = door;
}



void initialiseLevel4() {
    const int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(1, 3, 0, 2, true, dir_up),
        Platform(2, 6, 1, 0, true, dir_up),
        Platform(2, 8, 0, 0, true, dir_right),
        Platform(4, 1, 1, 0, true, dir_left),
    };
    
    game.nTilesX = 5;
    game.nTilesY = 9;
    game.player = Player(2, 2);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.8;
    Door door = Door(3, 0, dir_up);
    game.door = door;
}


void initialiseLevel5() {
    const int nPlats = 7;
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
    game.player = Player(3, 0);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.9;
    Door door = Door(3, 7, dir_down);
    game.door = door;
}

void initialiseLevel6() {
    const int nPlats = 5;
    Platform plats[nPlats] = {
        Platform(3, 2, 1, 1, true, dir_up),
        Platform(0, 4, 1, 1, true, dir_right),
        Platform(4, 3, 0, 3, true, dir_up),
        Platform(3, 6, 0, 2, true, dir_left),
        Platform(6, 6, 0, 1, true, dir_up),
    };
    
    game.nTilesX = 8;
    game.nTilesY = 8;
    game.player = Player(3, 0);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.9;
    Door door = Door(5, 7, dir_left);
    game.door = door;
}


void initialiseLevel7() {
    const int nPlats = 10;
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
    game.player = Player(3, 4);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.7;
    Door door = Door(6, 12, dir_right);
    game.door = door;
}


void initialiseLevel8() {
    const int nPlats = 16;
    Platform plats[nPlats] = {
        Platform(2, 3, 1, 1, true, dir_right),
        Platform(5, 2, 1, 1, true, dir_right),
        Platform(5, 4, 1, 1, false, dir_up),
        Platform(7, 4, 0, 3, true, dir_up),
        Platform(13, 2, 1, 1, true, dir_down),
        Platform(15, 4, 1, 1, true, dir_right),
        Platform(18, 8, 1, 1, true, dir_right),
        Platform(13, 9, 1, 1, true, dir_up),
        Platform(11, 12, 1, 1, true, dir_up),
        Platform(7, 13, 1, 1, true, dir_left),
        Platform(6, 6, 2, 2, false, dir_right),
        Platform(2, 14, 1, 1, true, dir_up),
        Platform(3, 12, 1, 1, true, dir_down),
        Platform(5, 10, 1, 1, true, dir_down),
        Platform(4, 6, 1, 1, false, dir_right),
        Platform(4, 8, 0, 1, true, dir_down)
    };
    
    game.nTilesX = 20;
    game.nTilesY = 17;
    game.player = Player(8, 2);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.7;
    Door door = Door(5, 6, dir_right);
    game.door = door;
}


void initialiseLevel9() {
    const int nPlats = 16;
    Platform plats[nPlats] = {
        Platform(2, 1, 0, 0, true, dir_down),
        Platform(3, 2, 1, 1, true, dir_right),
        Platform(5, 1, 1, 1, true, dir_up),
        Platform(1, 5, 2, 0, true, dir_down),
        Platform(5, 4, 1, 1, true, dir_left),
        Platform(7, 3, 1, 1, true, dir_up),
        Platform(9, 2, 0, 2, true, dir_right),
        Platform(11, 5, 0, 2, true, dir_left),
        Platform(12, 6, 0, 0, false, dir_left),
        Platform(9, 6, 2, 0, true, dir_up),
        Platform(1, 8, 0, 1, true, dir_left),
        Platform(5, 7, 1, 1, true, dir_up),
        Platform(3, 9, 1, 1, true, dir_down),
        Platform(6, 10, 2, 0, true, dir_right),
        Platform(8, 10, 0, 0, true, dir_left),
        Platform(10, 1, 0, 0, true, dir_up),
    };
    
    game.nTilesX = 13;
    game.nTilesY = 12;
    game.player = Player(5, 0);
    // game.player.setOrientation(dir_right);
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    game.zoom = 0.9;
    Door door = Door(11, 1, dir_up);
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
        case 6: initialiseLevel6(); break;
        case 7: initialiseLevel7(); break;
        case 8: initialiseLevel8(); break;
        case 9: initialiseLevel9(); break;
    }

    game.camera = Camera(&game.player);
    game.width = game.nTilesX*TILE_WIDTH;
    game.height = game.nTilesY*TILE_WIDTH;
    Background bg = Background(game.nTilesX / 2, game.nTilesY / 2);
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

    sf::Font comicsans;
    if (!comicsans.loadFromFile("comicbd.ttf")) std::cout << "ERROR\n";
    
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
        numbering.setFont(comicsans);
        numbering.setString(ss.str());
        numbering.setCharacterSize(60);
        numbering.setColor(textures->platformColor);
        numbering.setPosition(itemSpacing*(col+0.5f)-halfItemWidth, itemSpacing*(row+0.5f)-halfItemWidth);
        window->draw(numbering);
    }
}

int main(int argc, TCHAR *argv[]) {
    sf::RenderWindow w(sf::VideoMode(RES_X, RES_Y), "Maze of Dat");
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
            
            if (inMenu) updateMenu();
            else updateGame();

            if (dTime < frameTime) {
                // frame skip if lagging
                window->clear();
                if (inMenu) drawMenuFrame();
                else drawGameFrame();
                window->display();
            }
        }
    }

    return 0;
}