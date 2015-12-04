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
    int nPlats = 2;
    Platform plats[nPlats] = { Platform(5, 5, 3, 4, true, dir_up), Platform(1, 1, 1, 2, false, dir_down) };
    
    game.nTilesX = 10;
    game.nTilesY = 10;
    game.player = Player();
    gridToActual(8, 3, &game.player.x, &game.player.y);
    std::cout << "actual x = " << game.player.x << "\n";
    std::cout << "actual y = " << game.player.y << "\n";
    game.platforms.assign(plats, plats + (size_t) nPlats);
}

void initialiseGame() {
    game = GameGlobals();
    initialiseLevel1();
    game.width = game.nTilesX*TILE_WIDTH;
    game.height = game.nTilesY*TILE_WIDTH;
    std::cout << "2actual x = " << game.player.x << "\n";
    std::cout << "2actual y = " << game.player.y << "\n";
    std::cout << "platform size = " << game.platforms.size() << "\n";
}

void updateGame() {
    game.key.update();
    game.player.update(game.key);
}

void drawGameFrame() {
    game.player.draw();
}

int main() {
    sf::Clock clock;

    float frameTime = 1/60.f;
    float dTime = 0;

    initialiseGame();
    std::cout << "3actual x = " << game.player.x << "\n";
    std::cout << "3actual y = " << game.player.y << "\n";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        if (dTime > frameTime) {
            dTime -= frameTime;
            //std::cout << "4actual x = " << game.player.x << "\n";
            //std::cout << "4actual y = " << game.player.y << "\n";

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