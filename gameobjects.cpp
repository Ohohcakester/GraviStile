#include <SFML/Graphics.hpp>
#include "gameobjects.h"
#include "keyboard.h"
#include "globals.h"

void IGameObject::drawCircle(sf::CircleShape shape, int px, int py) {
    shape.setOrigin(-px+shape.getRadius(),-py+shape.getRadius());
    window.draw(shape);
}




Player::Player() {
    x = WIDTH/2; y = HEIGHT/2;
    speed = 10;
    shape = sf::CircleShape(50);
    shape.setFillColor(sf::Color::Green);
}

void Player::draw() {
    drawCircle(shape, x, y);
}

void Player::update(Keyboard k) {
    vx = 0; vy = 0;
    if (k.down) vy += speed;
    if (k.up) vy -= speed;
    if (k.left) vx -= speed;
    if (k.right) vx += speed;

    x += vx;
    y += vy;
}