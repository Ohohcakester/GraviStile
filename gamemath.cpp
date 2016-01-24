
#define _USE_MATH_DEFINES
#include <math.h>

#include "gamemath.h"
#include "Point.h"

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

void generateRotatedCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry, float* tr_x, float* tr_y, float angle) {
    *tlx = x1;
    *tly = y1;
    *blx = x1;
    *bly = y2;
    *brx = x2;
    *bry = y2;
    *tr_x = x2;
    *tr_y = y1;

    rotateVector(tlx, tly, angle);
    rotateVector(blx, bly, angle);
    rotateVector(brx, bry, angle);
    rotateVector(tr_x, tr_y, angle);
}

// clamped between -PI and PI.
float clampedAngularDifference(float fromAngle, float toAngle) {
    float diff = toAngle - fromAngle;
    while (diff <= -M_PI) diff += M_2PI;
    while (diff > M_PI) diff -= M_2PI;
    return diff;
}

// clamped between -PI and PI.
float clampedAngle(float angle) {
    while (angle <= -M_PI) angle += M_2PI;
    while (angle > M_PI) angle -= M_2PI;
    return angle;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

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

bool isWithinRect(int x, int y, int x1, int y1, int x2, int y2) {
    return (x > x1 && y > y1 && x < x2 && y < y2);
}

float dotProduct(float dx1, float dy1, float dx2, float dy2) {
    return dx1*dx2 + dy1*dy2;
}

int div_floor(int n, int d) {
    int q = n / d;
    int r = n%d;
    if ((r != 0) && ((r < 0) != (d < 0)))--q;
    return q;
}