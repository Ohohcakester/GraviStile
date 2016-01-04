#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <SFML/Graphics.hpp>
class Point;
class SpinConnection;
class LaserSource;
class LaserTarget;
#include "IGameObject.h"

enum platformDisabledStatus {
    platformStatus_enabled,
    platformStatus_disabled,
    platformStatus_waitingForEnable,
};

class Platform : public IGameObject {
    void Platform::updateUsingDisabledGraphic();

public:
    sf::RectangleShape shape;
    sf::CircleShape pivotShape;
    sf::Sprite sprite;
    sf::RectangleShape extraLineShape;

    bool isUsingDisabledGraphic;
    int disabledStatus;
    bool isRotationDisabled;

    SpinConnection* spinConnection;
    std::vector<LaserSource*> laserSources;
    std::vector<LaserTarget*> laserTargets;

    bool isNull;
    int cx;
    int cy;
    int leftTiles;
    int rightTiles;
    bool rotatable;
    int orientation;

    float rotateSpeed;
    float angle;
    float targetAngle;
    bool isRotating;

    // rect
    int x1;
    int y1;
    int x2;
    int y2;

    Platform();

    Platform(int pivotX, int pivotY, int leftTiles, int rightTiles, bool rotatable, int orientation, bool isDisabled = false, bool isRotationDisabled = false);

    virtual void draw();

    virtual void update(Keyboard k);

    void onReach();

    void rotateTo(int newOrientation);

    virtual void setOrientation(int orientation);

    bool isUnderDoor(int cx, int cy);

    virtual bool sweep(bool right);

    virtual bool platCheck(int leftQuad, int rightQuad, bool vertFlip, Platform* other);

    virtual bool twoPointsTwoDistances(Point center, int p1x, int p1y, int p2x, int p2y, int lQuad, int rQuad, bool quads24);

    bool isDisabled();

    bool samePosition(Platform* other);

    void toggleDisabledStatus();
    void toggleRotationDisabledStatus();

    void temporarilyDisableLaserSources();
    void repositionAttachedObjects();

    void addLaserSource(LaserSource* laserSource);
    void addLaserTarget(LaserTarget* laserTarget);
};


#endif