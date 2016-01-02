#include <tchar.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "keyboard.h"
#include "gameobjects.h"
#include "globals.h"
#include "Stage.h"

bool inMenu = true;

void initialiseGame(int stage);
void initialiseMenu();

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

void keyPress(sf::Keyboard::Key keyCode) {
    //std::cout << "Press " << keyCode << std::endl;
    if (inMenu) {
        if (keyCode == sf::Keyboard::Space) initialiseGame(menu.selection + 1);
        if (keyCode == sf::Keyboard::Left) menu.previous();
        if (keyCode == sf::Keyboard::Right) menu.next();
        if (keyCode == sf::Keyboard::Up) menu.up();
        if (keyCode == sf::Keyboard::Down) menu.down();
    } else {
        if (keyCode == sf::Keyboard::Space) game.player.jump();
        if (keyCode == sf::Keyboard::A) rotateLeft();
        if (keyCode == sf::Keyboard::D) rotateRight();
        if (keyCode == sf::Keyboard::R) initialiseGame(game.currentStage);
        if (keyCode == sf::Keyboard::Escape) initialiseMenu();
        //if (keyCode == sf::Keyboard::T) game.player.currentPlatform->disable(); for debugging.
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


void initialiseGame(int stage) {
    inMenu = false;
    game = GameGlobals();
    game.puzzleComplete = false;
    game.zoom = 0.4;

    initialiseStage(stage);

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
        game.platforms[i]->update(game.key);
    }
    game.door.update(game.key);
}

void updateMenu() {

}

void drawGameFrame() {
    game.background.draw();
    game.camera.draw();
    for (int i=0;i<game.platforms.size();++i) {
        game.platforms[i]->draw();
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