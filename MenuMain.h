#ifndef MENU_MAIN_H_
#define MENU_MAIN_H_

#include <SFML/Graphics.hpp>

sf::Text setupText(int cx, int cy, int size, sf::Color color, std::string text);

void initialiseMenu();

void drawMenuFrame();

void menuKeyPress(sf::Keyboard::Key keyCode);

void updateMenu();


#endif