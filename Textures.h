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

    sf::Color menuButtonFaceColor;
    sf::Color menuButtonFaceDisabledColor;
    sf::Color menuButtonBorderColor;
    sf::Color menuButtonBorderClearedColor;

    sf::Color spinConnectionWireColor;
    sf::Color switchConnectionWireColor;

    sf::Color levelNameTextColor;

    sf::Font comicsans;

    Textures();
};


#endif