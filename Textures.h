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

    Textures();
};


#endif