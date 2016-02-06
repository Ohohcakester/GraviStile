#include "Sfx.h"
#include "Textures.h"
#include "globals.h"
#include "gamemath.h"
#include <sstream>
#include <iostream>
#include "GameGlobals.h"

class Keyboard;

sfx::LevelName::LevelName(std::string name, int level) : 
Sfx(global::RES_X / 2, global::RES_Y / 10), text(name) {
    std::stringstream ss;
    ss << "Stage " << level;
    levelNumberText = ss.str();

    color = global::textures->levelNameTextColor;

    sfLevelText.setFont(global::textures->mainFont);
    sfLevelText.setString(levelNumberText);
    sfLevelText.setCharacterSize(20);
    sfLevelText.setColor(color);
    sf::FloatRect textRect = sfLevelText.getLocalBounds();
    sfLevelText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    sfText.setFont(global::textures->mainFont);
    sfText.setString(text);
    sfText.setCharacterSize(30);
    sfText.setColor(color);
    textRect = sfText.getLocalBounds();
    sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}

void sfx::LevelName::draw() {
    if (animframe > 80) color.a = (120 - animframe) * 255 / 40;
    if (animframe > 120) color.a = 0;

    sfLevelText.setColor(color);
    sfLevelText.setPosition(x, y-20);
    sfText.setColor(color);
    sfText.setPosition(x, y+15);

    global::window->draw(sfLevelText);
    global::window->draw(sfText);
}

void sfx::LevelName::sfxUpdate() {
    runPhysics();
    if (animframe == 80) vy = -1;
    if (animframe > 120) die();
}

sfx::Text::Text(int x, int y, std::string text, int timeout, int delay, int size) : 
Sfx(x, y), text(text), timeout(timeout), delay(delay) {
    color = sf::Color::White;

    sfText.setFont(global::textures->mainFont);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setColor(color);
    sf::FloatRect textRect = sfText.getLocalBounds();
    sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}

void sfx::Text::draw() {
    if (animframe >= delay) {
        sfText.setColor(color);
        sfText.setPosition(x, y);
        global::window->draw(sfText);
    }
}

void sfx::Text::sfxUpdate() {
    if (timeout != -1 && animframe > timeout) die();
}


sfx::PlayerDeath::PlayerDeath(int x, int y, float angle, float pheight, bool facingRight) :
        Sfx(x, y), angle(angle), baseSize(pheight/2), facingRight(facingRight) {
    sprite.setTexture(global::textures->player);
    size = baseSize;

    float ax = 0, ay = 0.05f;
    rotateVector(&ax, &ay, angle);
    for (int i = 0; i < 15; ++i) {
        float fireAngle = randomAngle();
        float vx = 0, vy = rand()%30/10.0 + 1;
        rotateVector(&vx, &vy, fireAngle);
        global::game.spawnNewSfx(new sfx::DeathParticle(x, y, vx, vy, ax, ay));
    }

}

void sfx::PlayerDeath::draw() {
    float tlx, tly, blx, bly, brx, bry;
    generateRotatedCorners(-size, -size, size, size, &tlx, &tly, &blx, &bly, &brx, &bry, angle);
    drawSprite(&sprite, x + tlx, y + tly, x + blx, y + bly, x + brx, y + bry, !facingRight);
}

void sfx::PlayerDeath::sfxUpdate() {
    size = baseSize * (30 - animframe) / 30;
    angle += 0.1f;
    if (animframe > 30) die();
}


sfx::DeathParticle::DeathParticle(int x, int y, float vx, float vy, float ax, float ay) :
Sfx(x, y, vx, vy, ax, ay), timeLimit(40) {
    color = sf::Color::White;
    shape = sf::CircleShape(global::game.zoom*4.0f);
    shape.setFillColor(color);
}

void sfx::DeathParticle::draw() {
    shape.setFillColor(color);
    drawCircle(&shape, x, y);
}

void sfx::DeathParticle::sfxUpdate() {
    runPhysics();
    color.a = (timeLimit - animframe) * 255 / timeLimit;
    if (animframe > timeLimit) color.a = 0;

    if (animframe > timeLimit) die();
}


sfx::LaserParticle::LaserParticle(int x, int y, float vx, float vy, float ax, float ay) :
Sfx(x, y, vx, vy, ax, ay), timeLimit(25) {
    color = sf::Color::Red;
    shape = sf::CircleShape(global::game.zoom*5.5f);
    shape.setFillColor(color);
}

void sfx::LaserParticle::draw() {
    shape.setFillColor(color);
    drawCircle(&shape, x, y);
}

void sfx::LaserParticle::sfxUpdate() {
    runPhysics();
    color.a = (timeLimit - animframe) * 191 / timeLimit;
    if (animframe > timeLimit) color.a = 0;

    if (animframe > timeLimit) die();
}


sfx::PlatformChange::PlatformChange(int x, int y, bool expanding) : Sfx(x, y), expanding(expanding), timeLimit(25), angle(0), scale(1) {
    color = sf::Color::Magenta;
    shape = sf::CircleShape(global::game.zoom*15.0f, 3);
    sfxUpdate();
    shape.setFillColor(color);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
}

void sfx::PlatformChange::draw() {
    shape.setFillColor(color);
    shape.setScale(sf::Vector2f(scale, scale));
    drawTriangle(&shape, x, y, angle);
}

void sfx::PlatformChange::sfxUpdate() {
    runPhysics();

    if (expanding) {
        angle += 0.15f;
        scale = animframe * 5.0f / timeLimit;
        color.a = (timeLimit - animframe) * 240 / timeLimit;
        if (animframe > timeLimit) color.a = 0;
    }
    else {
        angle -= 0.15f;
        scale = (timeLimit - animframe) * 5.0f / timeLimit;
        color.a = animframe * 240 / timeLimit;
        if (animframe > timeLimit) color.a = 240;
    }

    if (animframe > timeLimit) die();
}


// Base Sfx Methods
void sfx::Sfx::update(Keyboard k) {
    if (!isActive) return;
    sfxUpdate();
    animframe++;
}

void sfx::Sfx::die() {
    isActive = false;
}

void sfx::Sfx::runPhysics() {
    vx += ax;
    vy += ay;
    x += vx;
    y += vy;
}