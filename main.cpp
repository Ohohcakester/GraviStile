#include <SFML/Graphics.hpp>
#include <iostream>
#include "keyboard.h"
#include "gameobjects.h"
#include "globals.h"

void keyPress(sf::Keyboard::Key keyCode) {
    std::cout << "Press " << keyCode << std::endl;
    if (keyCode == sf::Keyboard::Space) game.player.jump();
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

void initialiseLevel1() {
    int nPlats = 4;
    Platform plats[nPlats] = {
        Platform(8, 7, 3, 4, true, dir_up),
        Platform(1, 6, 1, 2, false, dir_down),
        Platform(8, 10, 5, 6, true, dir_up),
        Platform(16, 11, 1, 6, true, dir_right)
    };
    
    game.nTilesX = 10;
    game.nTilesY = 10;
    game.player = Player();
    gridToActual(8, 3, &game.player.x, &game.player.y);
    game.platforms.assign(plats, plats + (size_t) nPlats);
}

void initialiseGame() {
    game = GameGlobals();
    initialiseLevel1();
    game.camera = Camera(&game.player);
    game.width = game.nTilesX*TILE_WIDTH;
    game.height = game.nTilesY*TILE_WIDTH;
}

void updateGame() {
    game.key.update();
    game.player.update(game.key);
    game.camera.update(game.key);
    for (int i=0;i<game.platforms.size();++i) {
        game.platforms[i].update(game.key);
    }
    game.door.update(game.key);
}

void drawGameFrame() {
    game.player.draw();
    game.camera.draw();
    for (int i=0;i<game.platforms.size();++i) {
        game.platforms[i].draw();
    }
    game.door.draw();
}

int main() {
    sf::Clock clock;

    float frameTime = 1/60.f;
    float dTime = 0;

    initialiseGame();
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        if (dTime > frameTime) {
            dTime -= frameTime;
            
            updateGame();

            if (dTime < frameTime) {
                // frame skip if lagging
                window.clear();
                drawGameFrame();
                window.display();
            }
        }
    }

    return 0;
}