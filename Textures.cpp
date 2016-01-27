#include <iostream>
#include "Textures.h"

Textures::Textures() {
    if (!mainFont.loadFromFile("VeraBd.ttf")) {
        std::cout << "Unable to load font!\n";
    }
    if (!background.loadFromFile("img/bg.jpg")) {
        std::cout << "Unable to load background!";
    }
    if (!player.loadFromFile("img/player.png")) {
        std::cout << "Unable to load player!";
    }
    if (!door.loadFromFile("img/door.png")) {
        std::cout << "Unable to load door!";
    }
    if (!pivot.loadFromFile("img/pivot.png")) {
        std::cout << "Unable to load pivot!";
    }

    platformColor = sf::Color(110, 63, 75, 255);
    platformSurfaceColor = sf::Color(255, 191, 196, 255);
    platformDisabledColor = sf::Color(110, 63, 75, 64);
    platformDisabledSurfaceColor = sf::Color(255, 191, 196, 64);

    laserSourceColor = sf::Color(191, 160, 224, 255);
    laserTargetColor = sf::Color(127, 127, 127, 255);
    laserColor = sf::Color(255, 0, 0, 255);

    menuButtonFaceColor = sf::Color(204, 140, 146, 255);
    menuButtonFaceDisabledColor = sf::Color(120, 80, 84, 255);
    menuButtonSurfaceColor = sf::Color(255, 191, 196, 255);
    menuButtonSurfaceDisabledColor = sf::Color(155, 100, 106, 255);
    menuButtonBorderColor = sf::Color(110, 63, 75, 255);
    menuButtonBorderClearedColor = sf::Color(127, 96, 60, 255);

    levelButtonTextClearedColor = sf::Color(96, 80, 60, 255);

    spinConnectionWireColor = sf::Color(255, 255, 0, 255);
    switchConnectionWireColor = sf::Color(96, 127, 0, 160);

    levelNameTextColor = sf::Color(255, 224, 127, 255);
}
