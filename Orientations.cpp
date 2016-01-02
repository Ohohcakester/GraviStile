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