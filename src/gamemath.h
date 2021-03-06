#ifndef GAMEMATH_H_
#define GAMEMATH_H_

#define _USE_MATH_DEFINES
#include <math.h>

const double M_2PI = 2 * M_PI;

void rotateVector(float* dx, float* dy, float angle);

float orientationToAngle(int orientation);

void generateRotatedCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry, float angle);

void generateRotatedCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry, float* tr_x, float* tr_y, float angle);

float clampedAngularDifference(float fromAngle, float toAngle);

float clampedAngle(float angle);

void swap(int* a, int* b);

bool rectsIntersectSafe(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b);

bool rectsIntersect(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b);

bool isWithinRect(int x, int y, int x1, int y1, int x2, int y2);

bool isWithinRectSafe(int x, int y, int x1, int y1, int x2, int y2);

float dotProduct(float dx1, float dy1, float dx2, float dy2);

int div_floor(int n, int d);

float randomAngle();

float randRange(float a, float b);

#endif