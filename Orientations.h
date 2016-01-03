#ifndef ORIENTATIONS_H_
#define ORIENTATIONS_H_

enum {
    dir_up,
    dir_right,
    dir_down,
    dir_left
};

int orientationRotateRight(int orientation);

int orientationRotateLeft(int orientation);

int orientationRotate(int orientation, bool right);

void orientationToMoveDirection(int orientation, int* dx, int* dy);

bool areComplementaryOrientations(int orientation1, int orientation2);

#endif