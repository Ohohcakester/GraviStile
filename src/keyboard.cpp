#include <SFML/Graphics.hpp>
#include "keyboard.h"

Keyboard::Keyboard() :
up(false), down(false), left(false), right(false), comma(false), period(false){}

void Keyboard::update() {
}

void Keyboard::updateKey(sf::Keyboard::Key key, bool status) {
    switch (key) {
    case sf::Keyboard::Up: up = status; break;
    case sf::Keyboard::Down: down = status; break;
    case sf::Keyboard::Left: left = status; break;
    case sf::Keyboard::Right: right = status; break;
    case sf::Keyboard::Comma: comma = status; break;
    case sf::Keyboard::Period: period = status; break;
    }
}