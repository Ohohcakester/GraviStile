#include "globals.h"
#include "Point.h"
#include "Platform.h"

Platform::Platform() {
    this->isNull = true;
}

Platform::Platform(int cx, int cy, int leftTiles, int rightTiles, bool rotatable, int orientation) {
    freeze = false;

    this->isNull = false;
    this->cx = cx;
    this->cy = cy;
    gridToActual(cx, cy, &this->x, &this->y);
    this->leftTiles = leftTiles;
    this->rightTiles = rightTiles;
    this->rotatable = rotatable;
    setOrientation(orientation);

    sprite.setTexture(textures->pivot);
    shape = sf::RectangleShape();
    shape.setFillColor(textures->platformColor);
    extraLineShape = sf::RectangleShape();
    extraLineShape.setFillColor(textures->platformSurfaceColor);
    pivotShape = sf::CircleShape(game.zoom*TILE_WIDTH / 3);
    pivotShape.setFillColor(sf::Color::Magenta);
}

void Platform::draw() {
    drawRectangle(&shape, x1, y1, x1, y2, x2, y2);

    if (this->rotatable) {
        int fraction = 5;
        int e_x1 = x1; int e_x2 = x2; int e_y1 = y1; int e_y2 = y2;
        switch (this->orientation) {
        case dir_up:
            e_y2 = (y2 - y1) / fraction + y1;
            break;
        case dir_down:
            e_y1 = (y1 - y2) / fraction + y2;
            break;
        case dir_left:
            e_x2 = (x2 - x1) / fraction + x1;
            break;
        case dir_right:
            e_x1 = (x1 - x2) / fraction + x2;
            break;
        }
        drawRectangle(&extraLineShape, e_x1, e_y1, e_x1, e_y2, e_x2, e_y2);

        drawCircle(&pivotShape, x, y);

        float radius = game.zoom*TILE_WIDTH / 3;
        drawSprite(&sprite, x - radius, y - radius, x - radius, y + radius, x + radius, y + radius);
    }
}

void Platform::update(Keyboard k) {
}

void Platform::setOrientation(int orientation) {
    this->orientation = orientation;

    this->x1 = x - TILE_WIDTH / 2;
    this->x2 = x + TILE_WIDTH / 2;
    this->y1 = y - TILE_WIDTH / 2;
    this->y2 = y + TILE_WIDTH / 2;

    if (orientation == dir_up) {
        this->x1 -= TILE_WIDTH * leftTiles;
        this->x2 += TILE_WIDTH * rightTiles;
    }

    if (orientation == dir_right) {
        this->y1 -= TILE_WIDTH * leftTiles;
        this->y2 += TILE_WIDTH * rightTiles;
    }

    if (orientation == dir_down) {
        this->x1 -= TILE_WIDTH * rightTiles;
        this->x2 += TILE_WIDTH * leftTiles;
    }

    if (orientation == dir_left) {
        this->y1 -= TILE_WIDTH * rightTiles;
        this->y2 += TILE_WIDTH * leftTiles;
    }
}

bool Platform::isUnderDoor(int doorX, int doorY) {
    switch (orientation) {
    case dir_up:
        return (cx - leftTiles <= doorX && doorX <= cx + rightTiles);
    case dir_down:
        return (cx - rightTiles <= doorX && doorX <= cx + leftTiles);
    case dir_left:
        return (cy - rightTiles <= doorY && doorY <= cy + leftTiles);
    case dir_right:
        return (cy - leftTiles <= doorY && doorY <= cy + rightTiles);
    }
    return false;
}

bool Platform::sweep(bool right) {
    int leftQuad;
    int rightQuad;
    if ((right && (orientation == dir_up || orientation == dir_down))
        || ((!right) && (orientation == dir_right || orientation == dir_left))) { // quads24
        switch (orientation) {
        case dir_up:
        case dir_right:
            leftQuad = TILE_WIDTH * leftTiles;
            rightQuad = TILE_WIDTH * rightTiles;
            break;
        case dir_down:
        case dir_left:
            leftQuad = TILE_WIDTH * rightTiles;
            rightQuad = TILE_WIDTH * leftTiles;
            break;
        }
        //std::cout << "right " << right << " orientation " << orientation;
        for (int i = 0; i<game.platforms.size(); ++i) {
            if (game.platforms[i].cx == cx && game.platforms[i].cy == cy) continue;
            //std::cout << "i = " << i << "\n";
            if (!platCheck(leftQuad, rightQuad, true, game.platforms[i])) return false;
        }
    }
    else { // quads13
        switch (orientation) {
        case dir_up:
        case dir_left:
            leftQuad = TILE_WIDTH * leftTiles;
            rightQuad = TILE_WIDTH * rightTiles;
            break;
        case dir_down:
        case dir_right:
            leftQuad = TILE_WIDTH * rightTiles;
            rightQuad = TILE_WIDTH * leftTiles;
            break;
        }
        //std::cout << "rightQuad " << rightQuad << "\n";
        for (int i = 0; i<game.platforms.size(); ++i) {
            if (game.platforms[i].cx == cx && game.platforms[i].cy == cy) continue;
            //std::cout << "i = " << i << "\n";
            if (!platCheck(leftQuad, rightQuad, false, game.platforms[i])) return false;
        }
    }
    return true;
}

bool Platform::platCheck(int leftQuad, int rightQuad, bool quads24, Platform other) { // quads24 ? quads 2-4 : quads 1-3
    Point centerPivot(x, y);
    int centerx1 = other.x - TILE_WIDTH / 2;
    int centerx2 = other.x + TILE_WIDTH / 2;
    int centery1 = other.y - TILE_WIDTH / 2;
    int centery2 = other.y + TILE_WIDTH / 2;

    switch (other.orientation) {
    case dir_up: {
        for (int i = 0; i < other.leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1 - TILE_WIDTH * i, centery1,
                centerx1 - TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other.rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx2 + TILE_WIDTH * i, centery1,
                centerx2 + TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    case dir_down: {
        for (int i = 0; i < other.leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx2 + TILE_WIDTH * i, centery1,
                centerx2 + TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other.rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1 - TILE_WIDTH * i, centery1,
                centerx1 - TILE_WIDTH * i, centery2,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    case dir_right: {
        for (int i = 0; i < other.leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery1 - TILE_WIDTH * i,
                centerx2, centery1 - TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other.rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery2 + TILE_WIDTH * i,
                centerx2, centery2 + TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    case dir_left: {
        for (int i = 0; i < other.leftTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery2 + TILE_WIDTH * i,
                centerx2, centery2 + TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        for (int i = 0; i < other.rightTiles + 1; ++i) {
            if (!twoPointsTwoDistances(centerPivot, centerx1, centery1 - TILE_WIDTH * i,
                centerx2, centery1 - TILE_WIDTH * i,
                leftQuad, rightQuad, quads24)) return false;
        }
        break;
    }
    }
    return true;
}

bool Platform::twoPointsTwoDistances(Point center, int p1x, int p1y, int p2x, int p2y, int lQuad, int rQuad, bool quads24) {
    Point one(p1x, p1y);
    Point two(p2x, p2y);
    if (quads24) {
        if (p1x < center.x && p1y < center.y && one.distance(center) < lQuad) {
            //std::cout << "a p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad2
        }
        if (p1x > center.x && p1y > center.y && one.distance(center) < rQuad) {
            //std::cout << "b p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad4
        }
        if (p2x < center.x && p2y < center.y && two.distance(center) < lQuad) {
            //std::cout << "c p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad2
        }
        if (p2x > center.x && p2y > center.y && two.distance(center) < rQuad) {
            //std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad4
        }
    }
    else {
        if (p1x < center.x && p1y > center.y && one.distance(center) < lQuad) {
            //std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad3
        }
        if (p1x > center.x && p1y < center.y && one.distance(center) < rQuad) {
            //std::cout << "e p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad1
        }
        if (p2x < center.x && p2y > center.y && two.distance(center) < lQuad) {
            //std::cout << "f p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            return false; // quad3
        }
        if (p2x > center.x && p2y < center.y && two.distance(center) < rQuad) {
            //std::cout << "g p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
            //std::cout << "First condition " << (p2x > center.x) << "\n";
            //std::cout << "Second condition " << (p2y < center.y) << "\n";
            //std::cout << "Distance from center " << two.distance(center) << "\n";
            //std::cout << "Spin radius " << rQuad << "\n";
            return false; // quad1
        }
    }
    /*
    if (quads24) {
    if (p1x < center.x && p1y > center.y && one.distance(center) < lQuad) {
    std::cout << "a p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad2
    }
    if (p1x > center.x && p1y < center.y && one.distance(center) < rQuad) {
    std::cout << "b p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad4
    }
    if (p2x < center.x && p2y > center.y && two.distance(center) < lQuad) {
    std::cout << "c p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad2
    }
    if (p2x > center.x && p2y < center.y && two.distance(center) < rQuad) {
    std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad4
    }
    } else {
    if (p1x < center.x && p1y < center.y && one.distance(center) < lQuad) {
    std::cout << "d p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad3
    }
    if (p1x > center.x && p1y > center.y && one.distance(center) < rQuad) {
    std::cout << "e p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad1
    }
    if (p2x < center.x && p2y < center.y && two.distance(center) < lQuad) {
    std::cout << "f p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad3
    }
    if (p2x > center.x && p2y > center.y && two.distance(center) < rQuad) {
    std::cout << "g p1x = " << p1x << " p1y = " << p1y << " p2x = " << p2x << " p2y = " << p2y << "\n";
    return false; // quad1
    }
    }*/
    return true;
}
