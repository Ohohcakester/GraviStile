#include <iostream>
#include <SFML/Graphics.hpp>
#include "LevelButton.h"
#include "Orientations.h"
#include "gamemath.h"
#include "globals.h"
#include "Menu.h"
#include "GameStats.h"
#include "Textures.h"
#include "MenuMain.h"
#include <sstream>

const float ROTATE_SPEED = 0.15f;

LevelButton::LevelButton(float cx, float cy, int stageNo) :
cx(cx), cy(cy), stageNo(stageNo), isRotating(true), angle(0), targetAngle(0) {

}

void LevelButton::update() {
    if (isRotating) {
        float dAngle = clampedAngle(targetAngle + 0.01f - angle) - 0.01f;
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


void LevelButton::draw() {
    Menu& menu = global::menu;
    GameStats& gameStats = global::gameStats;

    float degAngle = angle * 180 / M_PI;

    float itemSpacing = menu.itemSpacing;
    float itemGlowWidth = itemSpacing*0.7f;
    float halfItemGlowWidth = itemGlowWidth / 2;

    float itemOutlineWidth = itemSpacing*0.6f;
    float halfItemOutlineWidth = itemOutlineWidth / 2;

    float itemWidth = itemSpacing*0.5f;
    float halfItemWidth = itemWidth / 2;


    if (stageNo == global::menu.selection + 1) {
        sf::RectangleShape shape3;
        shape3.setRotation(degAngle);
        shape3.setFillColor(sf::Color::White);
        shape3.setSize(sf::Vector2f(itemGlowWidth, itemGlowWidth));
        shape3.setOrigin(halfItemGlowWidth, halfItemGlowWidth);
        shape3.setPosition(cx, cy);
        global::window->draw(shape3);
    }

    sf::RectangleShape shape;

    {
        if (gameStats.cleared[stageNo]) shape.setFillColor(global::textures->menuButtonBorderClearedColor);
        else shape.setFillColor(global::textures->menuButtonBorderColor);
        shape.setRotation(degAngle);
        shape.setSize(sf::Vector2f(itemOutlineWidth, itemOutlineWidth));
        shape.setOrigin(halfItemOutlineWidth, halfItemOutlineWidth);
        shape.setPosition(cx, cy);
        global::window->draw(shape);
    }

    {
        sf::RectangleShape shape2;
        if (gameStats.isLocked(stageNo)) shape2.setFillColor(global::textures->menuButtonFaceDisabledColor);
        else shape2.setFillColor(global::textures->menuButtonFaceColor);
        shape2.setRotation(degAngle);
        shape2.setSize(sf::Vector2f(itemWidth, itemWidth));
        shape2.setOrigin(halfItemWidth, halfItemWidth);
        shape2.setPosition(cx, cy);
        global::window->draw(shape2);
    }
    
    {
        sf::RectangleShape shape2;
        if (gameStats.isLocked(stageNo)) shape2.setFillColor(global::textures->menuButtonSurfaceDisabledColor);
        else shape2.setFillColor(global::textures->menuButtonSurfaceColor);
        shape2.setRotation(degAngle);
        shape2.setSize(sf::Vector2f(itemOutlineWidth, itemOutlineWidth / 6));
        shape2.setOrigin(halfItemOutlineWidth, halfItemOutlineWidth);
        shape2.setPosition(cx, cy);
        global::window->draw(shape2);
    }


    if (gameStats.cleared[stageNo]) {
        sf::Text sfText = setupText(cx, cy, 15, global::textures->levelButtonTextClearedColor, "CLEAR");
        sfText.setOrigin(sfText.getOrigin() + sf::Vector2f(0,-25));
        sfText.setRotation(degAngle);
        global::window->draw(sfText);
    }

    {
        std::ostringstream ss;
        ss << stageNo;
        sf::Text sfText = setupText(cx, cy, 40, global::textures->menuButtonBorderColor, ss.str());
        sfText.setOrigin(sfText.getOrigin() + sf::Vector2f(0, 5));
        sfText.setRotation(degAngle);
        global::window->draw(sfText);
    }
}

void LevelButton::rotateTo(int orientation) {
    targetAngle = orientationToAngle(orientation);
    isRotating = true;
}

void LevelButton::rotateToBetween(float weight1, int orientation1, int orientation2) {
    targetAngle = weight1*orientationToAngle(orientation1) + (1-weight1)*orientationToAngle(orientation2);
    isRotating = true;
}

void LevelButton::finishRotating() {
    angle = clampedAngle(targetAngle);

}