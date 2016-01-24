#ifndef TEST_STUBS_H_
#define TEST_HSTUBS_H_
#include "AbstractCamera.h"
class Keyboard;

namespace test {
    class StubCamera : public AbstractCamera {
        void update(Keyboard k) {

        }
    };


}

#endif