#include "main.h"
#include "GameGlobals.h"
#include "GameStats.h"
#include "globals.h"
#include "Menu.h"
#include "Textures.h"
#include <sstream>

void drawText(int cx, int cy, int size, sf::Color color, std::string text) {
    sf::Text sfText;
    sfText.setFont(global::textures->comicsans);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setColor(color);
    sf::FloatRect textRect = sfText.getLocalBounds();
    sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    sfText.setPosition(cx, cy);
    global::window->draw(sfText);
}


void initialiseMenu() {
    gameStatus = gamestatus_menu;
}

void updateMenu() {

}


void drawMainMenu() {
    drawText(global::RES_X / 2, global::RES_Y / 4, 50, global::textures->platformSurfaceColor, "GraviStile");
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

        float cx = itemSpacing*(col + 0.5f);
        float cy = itemSpacing*(row + 0.5f);

        if (i == menu.selection) {
            sf::RectangleShape shape3;
            shape3.setFillColor(sf::Color::White);
            shape3.setSize(sf::Vector2f(itemGlowWidth, itemGlowWidth));
            shape3.setPosition(cx - halfItemGlowWidth, cy - halfItemGlowWidth);
            global::window->draw(shape3);
        }

        sf::RectangleShape shape;

        if (gameStats.cleared[i + 1]) shape.setFillColor(global::textures->menuButtonBorderClearedColor);
        else shape.setFillColor(global::textures->menuButtonBorderColor);
        shape.setSize(sf::Vector2f(itemOutlineWidth, itemOutlineWidth));
        shape.setPosition(cx - halfItemOutlineWidth, cy - halfItemOutlineWidth);
        global::window->draw(shape);

        sf::RectangleShape shape2;
        if (gameStats.isLocked(i + 1)) shape2.setFillColor(global::textures->menuButtonFaceDisabledColor);
        else shape2.setFillColor(global::textures->menuButtonFaceColor);
        shape2.setSize(sf::Vector2f(itemWidth, itemWidth));
        shape2.setPosition(cx - halfItemWidth, cy - halfItemWidth);
        global::window->draw(shape2);

        if (gameStats.cleared[i + 1]) {
            drawText(cx, cy + 25, 15, global::textures->levelButtonTextClearedColor, "CLEAR");
        }

        std::ostringstream ss;
        ss << i + 1;
        drawText(cx, cy-5, 40, global::textures->menuButtonBorderColor, ss.str());
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