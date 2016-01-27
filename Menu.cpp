#include "Menu.h"
#include "globals.h"
#include "LevelButton.h"
#include "Orientations.h"
#include "GameStats.h"

#include <iostream>

Menu::Menu() : currentMenu(menu_main), isGameStarting(false) {
    cols = 6;
    nItems = global::NUMBER_OF_STAGES;
    selection = 0;

    setupLevelButtons();
    refreshLevelButtonRotations();
}

void Menu::initialise() {
    menuPlatform.initialise();
}

void Menu::setupLevelButtons() {
    levelButtons.reserve(nItems);

    int top_pad = global::RES_Y / 10;

    itemSpacing = global::RES_X / cols;
    for (int i = 0; i < nItems; ++i) {
        int col = i%cols;
        int row = i / cols;

        float cx = itemSpacing*(col + 0.5f);
        float cy = itemSpacing*(row + 0.5f) + top_pad;

        levelButtons.push_back(LevelButton(cx, cy, i + 1));
    }
}

void Menu::refreshLevelButtonRotations() {
    for (size_t i = 0, n = levelButtons.size(); i < n; ++i) {
        if (i == selection) {
            levelButtons[i].rotateToBetween(0.7f, dir_up, dir_right);
        }
        else {
            if (global::gameStats.cleared[i + 1]) {
                levelButtons[i].rotateTo(dir_up);
            }
            else {
                levelButtons[i].rotateTo(dir_right);
            }
        }
    }
}

void Menu::next() {
    selection++;
    if (selection >= nItems) selection = 0;
    refreshLevelButtonRotations();
}

void Menu::previous() {
    selection--;
    if (selection < 0) selection = nItems - 1;
    refreshLevelButtonRotations();
}

void Menu::down() {
    selection += cols;
    if (selection >= nItems) {
        selection %= cols;
    }
    refreshLevelButtonRotations();
}

void Menu::up() {
    selection -= cols;
    if (selection < 0) {
        selection += nItems / cols * cols + cols;
        if (selection >= nItems) selection -= cols;
    }
    refreshLevelButtonRotations();
}

void Menu::gotoMenu(int menuScreen) {
    currentMenu = menuScreen;
    switch (currentMenu) {
    case menu_main:
        menuPlatform.tryRotateTo(dir_up);
        break;
    case menu_levelSelect:
        break;
    }
}

void Menu::triggerStartGameSequence() {
    refreshLevelButtonRotations();
    levelButtons[selection].rotateToBetween(0.7f, dir_up, dir_left, true);
}