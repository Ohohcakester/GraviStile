#include <iostream>
#include "Textures.h"

Textures::Textures() {
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
    laserSourceColor = sf::Color(191, 160, 224, 255);
    laserTargetColor = sf::Color(127, 127, 127, 255);
    laserColor = sf::Color(255, 0, 0, 255);
    //std::cout << platformColor.r << " " << platformColor.g << " " << platformColor.b << std::endl;
}
