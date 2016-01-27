#ifndef LEVEL_BUTTON_H_
#define LEVEL_BUTTON_H_


class LevelButton {
public:
    int stageNo;

    LevelButton(float cx, float cy, int stageNo);

    void update();
    void draw();
    void rotateTo(int orientation);
    void rotateToBetween(float weight1, int orientation1, int orientation2);

protected:
    float cx;
    float cy;
    float angle;
    float targetAngle;
    bool isRotating;

    void finishRotating();
};



#endif