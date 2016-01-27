#include "main.h"
#include "GameGlobals.h"
#include "GameStats.h"
#include "globals.h"
#include "Menu.h"
#include "Textures.h"
#include <sstream>
#include "LevelButton.h"
#include <iostream>

sf::Text setupText(int cx, int cy, int size, sf::Color color, std::string text) {
    sf::Text sfText;
    sfText.setFont(global::textures->mainFont);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setColor(color);
    sf::FloatRect textRect = sfText.getLocalBounds();
    sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    sfText.setPosition(cx, cy);
    return sfText;
}


void initialiseMenu() {
    gameStatus = gamestatus_menu;
    global::menu.refreshLevelButtonRotations();
}

void updateMenu() {
    std::vector<LevelButton>& levelButtons = global::menu.levelButtons;
    for (size_t i = 0, n = levelButtons.size(); i < n; ++i) {
        levelButtons[i].update();
    }
    global::menu.menuPlatform.update();
}


void drawMainMenu() {
    sf::Sprite sprite;
    sprite.setTexture(global::textures->background);

    float fit = std::max(global::RES_X / sprite.getLocalBounds().width, global::RES_Y / sprite.getLocalBounds().height) * 1.5f;
    sprite.setScale(sf::Vector2f(fit, fit));
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setPosition(global::RES_X/2, global::RES_Y/3);
    global::window->draw(sprite);

    global::menu.menuPlatform.draw();

    sf::Text sfText = setupText(global::RES_X / 2, global::RES_Y / 4, 50, global::textures->platformSurfaceColor, "GraviStile");
    global::window->draw(sfText);
}

void drawLevelSelect() {
    std::vector<LevelButton>& levelButtons = global::menu.levelButtons;
    for (size_t i = 0, n = levelButtons.size(); i < n; ++i) {
        levelButtons[i].draw();
    }
}

void drawMenuFrame() {
    switch (global::menu.currentMenu) {
    case menu_main:
        drawMainMenu();
        break;
    case menu_levelSelect:
        drawLevelSelect();
        break;
    }
}

bool tryStartStage(int stageNo) {
    if (global::gameStats.isLocked(stageNo)) return false;
    initialiseGame(stageNo);
    return true;
}


void menuLevelSelectKeyPress(sf::Keyboard::Key keyCode) {
    Menu& menu = global::menu;
    if (menu.isGameStarting) return;

    if (keyCode == sf::Keyboard::Return) menu.triggerStartGameSequence();
    if (keyCode == sf::Keyboard::Space) menu.triggerStartGameSequence();
    if (keyCode == sf::Keyboard::A) menu.triggerStartGameSequence();
    if (keyCode == sf::Keyboard::Left) menu.previous();
    if (keyCode == sf::Keyboard::Right) menu.next();
    if (keyCode == sf::Keyboard::Up) menu.up();
    if (keyCode == sf::Keyboard::Down) menu.down();

    if (keyCode == sf::Keyboard::Escape) global::menu.gotoMenu(menu_main);

    /*if (keyCode == sf::Keyboard::Num1) initialiseGame(100);
    if (keyCode == sf::Keyboard::Num2) initialiseGame(101);
    if (keyCode == sf::Keyboard::Num3) initialiseGame(102);

    if (keyCode == sf::Keyboard::BackSpace) initialiseEditor();*/
}

void menuMainKeyPress(sf::Keyboard::Key keyCode) {
    Menu& menu = global::menu;

    if (keyCode == sf::Keyboard::A) global::menu.menuPlatform.tryRotateTo(dir_left);
    if (keyCode == sf::Keyboard::D) global::menu.menuPlatform.tryRotateTo(dir_right);
}

void menuKeyPress(sf::Keyboard::Key keyCode) {
    switch (global::menu.currentMenu) {
    case menu_main:
        menuMainKeyPress(keyCode);
        break;
    case menu_levelSelect:
        menuLevelSelectKeyPress(keyCode);
        break;
    }
}