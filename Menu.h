#ifndef MENU_H_
#define MENU_H_

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

    Menu();
    void next();
    void previous();
    void down();
    void up();

    void gotoMenu(int menuScreen);
};

#endif