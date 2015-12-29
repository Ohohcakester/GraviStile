
#define _USE_MATH_DEFINES
#include <math.h>

#include "gamemath.h"

/// Angle in radians.
void rotateVector(float* dx, float* dy, float angle) {
    float ndx = *dx * cos(angle) - *dy * sin(angle);
    float ndy = *dx * sin(angle) + *dy * cos(angle);
    *dx = ndx;
    *dy = ndy;
}

float orientationToAngle(int orientation) {
    return orientation * M_PI_2;
}


void generateRotatedCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry, float angle) {
    *tlx = x1;
    *tly = y1;
    *blx = x1;
    *bly = y2;
    *brx = x2;
    *bry = y2;

    rotateVector(tlx, tly, angle);
    rotateVector(blx, bly, angle);
    rotateVector(brx, bry, angle);
}

// clamped between -180 and 180.
float clampedAngularDifference(float fromAngle, float toAngle) {
    float diff = toAngle - fromAngle;
    while (diff <= -M_PI) diff += M_2PI;
    while (diff > M_PI) diff -= M_2PI;
    return diff;
}
