#ifndef MENU_PLATFORM_H_
#define MENU_PLATFORM_H_

#include <SFML/Graphics.hpp>

class MenuPlatform {
public:
    MenuPlatform();
    void initialise();

    void update();
    void finishRotating();

    void draw();

    void tryRotateTo(int orientation);

protected:
    float cx;
    float cy;
    float angle;
    float targetAngle;
    bool isRotating;
    int orientation;

    sf::RectangleShape rectShape;
    sf::RectangleShape surfaceShape;
    sf::Sprite pivotShape;
    sf::Sprite playerSprite;

    int transitionRemainingFrames;

    void rotateTo(int orientation);
};


#endif