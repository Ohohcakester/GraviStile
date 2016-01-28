#ifndef GAME_STATS_H_
#define GAME_STATS_H_

#include <vector>

class GameStats {
public:
    std::vector<bool> cleared;
    int lastUnlockedStage;

    GameStats();
    void refreshLastUnlockedStage();
    bool isLocked(int stageNo);
    void stageClear(int stageNo);

    void saveData();
    void loadData();
};



#endif