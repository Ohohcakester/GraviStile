#include "Sfx.h"
#include "Textures.h"
#include "globals.h"
#include "gamemath.h"
#include <sstream>
#include <iostream>
#include "GameGlobals.h"

class Keyboard;

sfx::LevelName::LevelName(std::string name, int level): Sfx(RES_X/2, RES_Y/3), text(name) {
    std::stringstream ss;
    ss << "Stage " << level;
    levelNumberText = ss.str();

    color = textures->levelNameTextColor;

    sfLevelText.setFont(textures->comicsans);
    sfLevelText.setString(levelNumberText);
    sfLevelText.setCharacterSize(20);
    sfLevelText.setColor(color);
    sf::FloatRect textRect = sfLevelText.getLocalBounds();
    sfLevelText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    sfText.setFont(textures->comicsans);
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
    sfText.setPosition(x, y+20);

    window->draw(sfLevelText);
    window->draw(sfText);
}

void sfx::LevelName::sfxUpdate() {
    runPhysics();
    if (animframe == 80) vy = -1;
    if (animframe > 120) die();
}

void sfx::Text::draw() {

}

void sfx::Text::sfxUpdate() {
    if (animframe > timeout) die();
}


sfx::PlayerDeath::PlayerDeath(int x, int y, float angle, float pheight, bool facingRight) :
        Sfx(x, y), angle(angle), baseSize(pheight/2), facingRight(facingRight) {
    sprite.setTexture(textures->player);
    size = baseSize;

    float ax = 0, ay = 0.05f;
    rotateVector(&ax, &ay, angle);
    for (int i = 0; i < 15; ++i) {
        float fireAngle = rand()%5000 / 5000.0 * M_2PI;
        float vx = 0, vy = rand()%30/10.0 + 1;
        rotateVector(&vx, &vy, fireAngle);
        game.spawnNewSfx(new sfx::DeathParticle(x, y, vx, vy, ax, ay));
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
    shape = sf::CircleShape(game.zoom*4.0f);
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