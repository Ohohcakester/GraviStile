#ifndef MENU_H_
#define MENU_H_

#include <vector>
#include "LevelButton.h"
#include "MenuPlatform.h"


enum eMenuScreen{
    menu_levelSelect,
    menu_main,
};

class Menu {
public:
    // Variables
    int currentMenu;
    int selection;

    // Constants
    int cols;
    int nItems;
    float itemSpacing;

    MenuPlatform menuPlatform;
    std::vector<LevelButton> levelButtons;

    Menu();
    void initialise();
    void next();
    void previous();
    void down();
    void up();

    void setupLevelButtons();
    void refreshLevelButtonRotations();

    void gotoMenu(int menuScreen);
};

#endif