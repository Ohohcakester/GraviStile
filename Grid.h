#ifndef GRID_H_
#define GRID_H_

#include <vector>
class Platform;

class Grid {
public:
    bool _isNull;
    int sizeY;
    int sizeX;
    int size;
    std::vector<bool> blocked;

    int minX, minY, maxX, maxY;

    Grid();
    Grid(int minX, int minY, int maxX, int maxY);
    bool isBlocked(int x, int y);
    void setBlocked(int x, int y, bool value);
    void toGridCoordinates(int* x, int* y);
    void toActualCoordinates(int* x, int* y);

    void reset();
    void addPlatform(Platform* platform);
    void addPlatforms(std::vector<Platform*>* platforms);
};






#endif