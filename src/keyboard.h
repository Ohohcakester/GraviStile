#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard {
public:
    bool up,down,left,right;
    bool comma, period;

    Keyboard();
    void update();
    void updateKey(sf::Keyboard::Key key, bool status);
};




#endif