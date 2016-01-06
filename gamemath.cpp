
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

// x1,y1 is the top left, x2,y2 is the bottom right.
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

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
#include <iostream>
bool rectsIntersectSafe(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b) {
    if (x2a < x1a) swap(&x1a, &x2a);
    if (y2a < y1a) swap(&y1a, &y2a);
    if (x2b < x1b) swap(&x1b, &x2b);
    if (y2b < y1b) swap(&y1b, &y2b);
    return (x2a > x1b && x2b > x1a && y2a > y1b && y2b > y1a);
}

bool rectsIntersect(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b) {
    return (x2a > x1b && x2b > x1a && y2a > y1b && y2b > y1a);
}