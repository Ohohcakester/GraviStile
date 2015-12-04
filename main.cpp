#include <SFML/Graphics.hpp>
#include <iostream>
#include "keyboard.h"
#include "gameobjects.h"
#include "globals.h"

void keyPress(sf::Keyboard::Key keyCode) {
    std::cout << "Press " << keyCode << std::endl;
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

void initialiseLevel1(GameGlobals game) {
    int nPlats = 2;
    Platform plats[nPlats] = { Platform(5, 5, 3, 4, true, dir_up), Platform(1, 1, 1, 2, false, dir_down) };
    
    game.player = Player();
    game.player.x = 2;
    game.player.y = 2;
    game.platforms.assign(plats, plats + (size_t) nPlats);
    
    /*
    for (std::vector<Platform>::iterator it = game.platforms.begin(); it != game.platforms.end(); ++it) {
        std::cout << "cx = " << it->cx << "\n";
    }*/
}

void initialiseGame() {
    game = GameGlobals();
    initialiseLevel1(game);
    game.width = game.nTilesX*TILE_WIDTH;
    game.height = game.nTilesY*TILE_WIDTH;
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