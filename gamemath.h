#ifndef GAMEMATH_H_
#define GAMEMATH_H_

#define _USE_MATH_DEFINES
#include <math.h>

const double M_2PI = 2 * M_PI;

void rotateVector(float* dx, float* dy, float angle);

float orientationToAngle(int orientation);

void generateRotatedCorners(float x1, float y1, float x2, float y2, float* tlx, float* tly, float* blx, float* bly, float* brx, float* bry, float angle);

float clampedAngularDifference(float fromAngle, float toAngle);

#endif