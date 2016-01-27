#include "main.h"
#include "GameGlobals.h"
#include "GameStats.h"
#include "globals.h"
#include "Menu.h"
#include "Textures.h"
#include <sstream>

void initialiseMenu() {
    gameStatus = gamestatus_menu;
}

void updateMenu() {

}


void drawMainMenu() {
    sf::Text numbering;
    numbering.setFont(global::textures->comicsans);
    numbering.setString("GraviStile");
    numbering.setCharacterSize(50);
    numbering.setColor(global::textures->platformSurfaceColor);
    sf::FloatRect textRect = numbering.getLocalBounds();
    numbering.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    numbering.setPosition(global::RES_X / 2, global::RES_Y / 4);
    global::window->draw(numbering);

}

void drawLevelSelect() {
    Menu& menu = global::menu;
    GameStats& gameStats = global::gameStats;

    float itemSpacing = global::RES_X / menu.cols;
    float itemGlowWidth = itemSpacing*0.7f;
    float halfItemGlowWidth = itemGlowWidth / 2;

    float itemOutlineWidth = itemSpacing*0.6f;
    float halfItemOutlineWidth = itemOutlineWidth / 2;

    float itemWidth = itemSpacing*0.5f;
    float halfItemWidth = itemWidth / 2;

    for (int i = 0; i<menu.nItems; ++i) {
        int col = i%menu.cols;
        int row = i / menu.cols;

        if (i == menu.selection) {
            sf::RectangleShape shape3;
            shape3.setFillColor(sf::Color::White);
            shape3.setSize(sf::Vector2f(itemGlowWidth, itemGlowWidth));
            shape3.setPosition(itemSpacing*(col + 0.5f) - halfItemGlowWidth, itemSpacing*(row + 0.5f) - halfItemGlowWidth);
            global::window->draw(shape3);
        }

        sf::RectangleShape shape;

        if (gameStats.cleared[i + 1]) shape.setFillColor(global::textures->menuButtonBorderClearedColor);
        else shape.setFillColor(global::textures->menuButtonBorderColor);
        shape.setSize(sf::Vector2f(itemOutlineWidth, itemOutlineWidth));
        shape.setPosition(itemSpacing*(col + 0.5f) - halfItemOutlineWidth, itemSpacing*(row + 0.5f) - halfItemOutlineWidth);
        global::window->draw(shape);

        sf::RectangleShape shape2;
        if (gameStats.isLocked(i + 1)) shape2.setFillColor(global::textures->menuButtonFaceDisabledColor);
        else shape2.setFillColor(global::textures->menuButtonFaceColor);
        shape2.setSize(sf::Vector2f(itemWidth, itemWidth));
        shape2.setPosition(itemSpacing*(col + 0.5f) - halfItemWidth, itemSpacing*(row + 0.5f) - halfItemWidth);
        global::window->draw(shape2);

        std::ostringstream ss;
        ss << i + 1;
        sf::Text numbering;
        numbering.setFont(global::textures->comicsans);
        numbering.setString(ss.str());
        numbering.setCharacterSize(40);
        numbering.setColor(global::textures->menuButtonBorderColor);
        sf::FloatRect textRect = numbering.getLocalBounds();
        numbering.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        numbering.setPosition(itemSpacing*(col + 0.5f), itemSpacing*(row + 0.5f));
        global::window->draw(numbering);
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

void tryStartStage(int stageNo) {
    if (global::gameStats.isLocked(stageNo)) return;
    initialiseGame(stageNo);
}


void menuLevelSelectKeyPress(sf::Keyboard::Key keyCode) {
    Menu& menu = global::menu;

    if (keyCode == sf::Keyboard::Return) tryStartStage(menu.selection + 1);
    if (keyCode == sf::Keyboard::Space) tryStartStage(menu.selection + 1);
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

    if (keyCode == sf::Keyboard::Return) global::menu.gotoMenu(menu_levelSelect);
    if (keyCode == sf::Keyboard::Space) global::menu.gotoMenu(menu_levelSelect);
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