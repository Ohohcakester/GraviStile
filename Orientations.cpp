#include "Orientations.h"

int orientationRotateRight(int orientation) {
    return (orientation + 1) % 4;
}

int orientationRotateLeft(int orientation) {
    return (orientation + 3) % 4;
}

int orientationRotate(int orientation, bool right) {
    if (right) return (orientation + 1) % 4;
    else return (orientation + 3) % 4;
}

void orientationToMoveDirection(int orientation, int* dx, int* dy) {
    switch (orientation) {
    case dir_up:
        *dx = 0;
        *dy = -1;
        return;
    case dir_right:
        *dx = 1;
        *dy = 0;
        return;
    case dir_down:
        *dx = 0;
        *dy = 1;
        return;
    case dir_left:
        *dx = -1;
        *dy = 0;
        return;
    }
}


bool areComplementaryOrientations(int orientation1, int orientation2) {
    return (orientation2 - orientation1 + 4) % 4 == 2;
}

std::string orientationName(int orientation) {
    switch (orientation) {
    case dir_up: return "dir_up";
    case dir_down: return "dir_down";
    case dir_left: return "dir_left";
    case dir_right: return "dir_right";
    }
}