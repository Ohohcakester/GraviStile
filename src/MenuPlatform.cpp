#include "MenuPlatform.h"
#include "Orientations.h"
#include "globals.h"
#include "gamemath.h"
#include "Textures.h"
#include "Menu.h"
#include <iostream>

const float ROTATE_SPEED = 0.1f;

MenuPlatform::MenuPlatform() :
cx(global::RES_X / 2), cy(global::RES_Y * 3 / 5), isRotating(true), angle(0), targetAngle(0), transitionRemainingFrames(-1) {
}

void MenuPlatform::initialise() {
    int tileWidth = 60;
    int width = 5 * tileWidth;
    int height = tileWidth;

    rectShape = sf::RectangleShape();
    rectShape.setFillColor(global::textures->platformColor);
    rectShape.setSize(sf::Vector2f(width, height));
    rectShape.setOrigin(width / 2, height / 2);
    rectShape.setPosition(cx, cy);

    surfaceShape = sf::RectangleShape();
    surfaceShape.setFillColor(global::textures->platformSurfaceColor);
    surfaceShape.setSize(sf::Vector2f(width, height / 5));
    surfaceShape.setOrigin(width / 2, height / 2);
    surfaceShape.setPosition(cx, cy);

    pivotShape.setTexture(global::textures->pivot);
    float scale = 40 / pivotShape.getLocalBounds().width;
    pivotShape.setScale(sf::Vector2f(scale, scale));
    pivotShape.setOrigin(pivotShape.getLocalBounds().width / 2, pivotShape.getLocalBounds().height / 2);
    pivotShape.setPosition(cx, cy);

    playerSprite.setTexture(global::textures->player);
    scale = 50 / playerSprite.getLocalBounds().height;
    playerSprite.setScale(sf::Vector2f(scale, scale));
    playerSprite.setOrigin(playerSprite.getLocalBounds().width / 2, playerSprite.getLocalBounds().height + height/2/scale);
    playerSprite.setPosition(cx, cy);
    
}

void MenuPlatform::update() {
    if (transitionRemainingFrames != -1) {
        transitionRemainingFrames--;
        if (transitionRemainingFrames <= 0) {
            transitionRemainingFrames = -1;
            global::menu.gotoMenu(menu_levelSelect);
        }
        return;
    }

    if (isRotating) {
        float dAngle = clampedAngle(targetAngle - angle);
        if (dAngle > 0) {
            if (dAngle > ROTATE_SPEED) {
                angle += ROTATE_SPEED;
            }
            else {
                finishRotating();
            }
        }
        else {
            if (-dAngle > ROTATE_SPEED) {
                angle -= ROTATE_SPEED;
            }
            else {
                finishRotating();
            }
        }
    }
}

void MenuPlatform::finishRotating() {
    isRotating = false;
    angle = targetAngle;
    if (orientation == dir_right || orientation == dir_left) {
        transitionRemainingFrames = 10;
    }
}

void MenuPlatform::tryRotateTo(int orientation) {
    if (transitionRemainingFrames != -1) return;
    rotateTo(orientation);
}

void MenuPlatform::rotateTo(int orientation) {
    this->orientation = orientation;
    targetAngle = orientationToAngle(orientation);
    isRotating = true;
}

void MenuPlatform::draw() {
    float degAngle = angle * 180 / M_PI;
    rectShape.setRotation(degAngle);
    surfaceShape.setRotation(degAngle);
    pivotShape.setRotation(degAngle);
    playerSprite.setRotation(degAngle);

    global::window->draw(rectShape);
    global::window->draw(surfaceShape);
    global::window->draw(pivotShape);
    global::window->draw(playerSprite);
}