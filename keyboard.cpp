#include <SFML/Graphics.hpp>
#include "keyboard.h"

Keyboard::Keyboard() {

}

void Keyboard::update() {
    up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    comma = sf::Keyboard::isKeyPressed(sf::Keyboard::Comma);
    period = sf::Keyboard::isKeyPressed(sf::Keyboard::Period);
}