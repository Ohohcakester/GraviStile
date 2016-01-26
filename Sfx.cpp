#include "Sfx.h"
#include "Textures.h"
#include "globals.h"
#include <sstream>

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