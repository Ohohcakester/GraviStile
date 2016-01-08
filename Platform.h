#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <SFML/Graphics.hpp>
class Point;
class Player;
class Grid;
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
    void updateUsingDisabledGraphic();
    bool oneSidedCollidesWith(Platform* o);

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
    int previousOrientation;
    int orientation;

    float angle;
    float targetAngle;
    bool isRotating;
    bool isRevertingToPreviousRotation;
    bool isRotationSuccessful;

    // rect
    int x1;
    int y1;
    int x2;
    int y2;

    Platform();

    Platform(int pivotX, int pivotY, int leftTiles, int rightTiles, bool rotatable, int orientation, bool isDisabled = false, bool isRotationDisabled = false);

    virtual void draw();

    virtual void update(Keyboard k);

    bool isObstructedWhileRotating();

    void finishRotation();
    void onReach();

    void cancelRotation();

    void rotateTo(int newOrientation);

    void saveCurrentOrientation();
    void revertToPreviousOrientation();

    virtual void setOrientation(int orientation);

    bool isUnderDoor(int cx, int cy);

    bool isDisabled();

    bool isBlockedFromUndisabling(Grid* grid, Player* player);
    bool tryUnblockFromDisabled(Grid* grid, Player* player);

    bool samePosition(Platform* other);

    void toggleDisabledStatus();
    void toggleRotationDisabledStatus();

    void temporarilyDisableLaserSources();
    void repositionAttachedObjects();

    void addLaserSource(LaserSource* laserSource);
    void addLaserTarget(LaserTarget* laserTarget);

    bool collidesWith(Platform* o);
};


#endif