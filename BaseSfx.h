#ifndef BASE_SFX_H_
#define BASE_SFX_H_

#include "IGameObject.h"
class Keyboard;

namespace sfx {
    class Sfx : public IGameObject {
    public:
        bool isActive;

        Sfx(int x, int y, float vx, float vy, float ax, float ay) : IGameObject(x, y), vx(vx), vy(vy), ax(ax), ay(ay), animframe(0), isActive(true) {}
        Sfx(int x, int y, float vx, float vy) : Sfx(x, y, vx, vy, 0, 0) {}
        Sfx(int x, int y) : Sfx(x, y, 0, 0) {}
        void die();
        void runPhysics();
        void update(Keyboard k);

    protected:
        virtual void sfxUpdate() = 0;
        int animframe;
        float vx, vy;
        float ax, ay;
    };
}


#endif