#include <iostream>
#include "teststubs.h"
#include "SaveIO.h"

using namespace test;

namespace test {
    void assertEqual(bool a, bool b) {
        if (a != b) std::cout << "NOT EQUAL " << a << " | " << b << "\n";
    }

    void assertEqual(int a, int b) {
        if (a != b) std::cout << "NOT EQUAL " << a << " | " << b << "\n";
    }

    void assertEqual(float a, float b) {
        if (abs(a - b) > 0.0001f) std::cout << "NOT EQUAL " << a << " | " << b << "\n";
    }

    void cameraRelAbsTest(float x, float y, float px, float py, float angle) {
        StubCamera camera;
        camera.angle = angle;
        camera.px = px;
        camera.py = py;

        float newX = x, newY = y;
        camera.toRel(&newX, &newY);
        camera.toAbs(&newX, &newY);

        assertEqual(x, newX);
        assertEqual(y, newY);
    }

    void saveDataTest(int size1, int size2) {
        int size = std::min(size1, size2);
        std::vector<bool> data(size1);
        for (int i = 0; i < size1; ++i) data[i] = rand() % 2;

        {
            SaveIO io(size1);
            for (int i = 0; i < size1; ++i) {
                io.data[i] = data[i];
            }
            io.save("TEST_SAVE.dat");
        }

        {
            SaveIO io(size2);
            io.load("TEST_SAVE.dat");

            for (int i = 0; i < size; ++i) {
                assertEqual(data[i], io.data[i]);
            }
        }

    }

    void runTests() {
        srand(time(NULL));
        std::cout << "Running Tests...\n";

        saveDataTest(30, 30);

        saveDataTest(20, 27);

        saveDataTest(13, 3);

        saveDataTest(2, 34);

        saveDataTest(89, 121);

        saveDataTest(127, 63);
        return;

        cameraRelAbsTest(-5, 3.2f, 15, -19.5f, 0);
        cameraRelAbsTest(0, 3.2, -0.03f, 3.2f, -1.2f);
        cameraRelAbsTest(10, 30.2f, 15, 19.5f, 712.0f);

        std::cout << "Finished Running Tests.\n";
    }
}