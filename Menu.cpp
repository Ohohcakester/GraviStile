#include "Menu.h"

Menu::Menu() {
    cols = 5;
    nItems = 12;
    selection = 0;
}

void Menu::next() {
    selection++;
    if (selection >= nItems) selection = 0;
}

void Menu::previous() {
    selection--;
    if (selection < 0) selection = nItems - 1;
}

void Menu::down() {
    selection += cols;
    if (selection >= nItems) {
        selection %= cols;
    }
}

void Menu::up() {
    selection -= cols;
    if (selection < 0) {
        selection += nItems / cols * cols + cols;
        if (selection >= nItems) selection -= cols;
    }
}

