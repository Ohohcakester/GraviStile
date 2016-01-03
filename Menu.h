#ifndef MENU_H_
#define MENU_H_

class Menu {
public:
    int cols;
    int nItems;
    int selection;

    Menu();
    void next();
    void previous();
    void down();
    void up();
};

#endif