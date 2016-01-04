#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <SFML/Graphics.hpp>

class Textures {
public:
    sf::Texture background;
    sf::Texture player;
    sf::Texture door;
    sf::Texture pivot;
    sf::Color platformColor;
    sf::Color platformSurfaceColor;
    sf::Color platformDisabledColor;
    sf::Color platformDisabledSurfaceColor;
    sf::Color laserSourceColor;
    sf::Color laserTargetColor;
    sf::Color laserColor;

    Textures();
};


#endif