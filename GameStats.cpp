#include "GameStats.h"
#include "globals.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const int UNLOCK_AHEAD = 5;
const string SAVE_FILE_NAME = "save.dat";

GameStats::GameStats() {
    cleared.resize(global::NUMBER_OF_STAGES+1, false);
    refreshLastUnlockedStage();
    loadData();
}

void GameStats::refreshLastUnlockedStage() {
    lastUnlockedStage = UNLOCK_AHEAD;
    for (size_t i = 1, n = cleared.size(); i < n; ++i) {
        if (cleared[i]) {
            lastUnlockedStage += 1;
        }
    }
}

void GameStats::stageClear(int stageNo) {
    cleared[stageNo] = true;
    refreshLastUnlockedStage();
    saveData();
}

bool GameStats::isLocked(int stageNo) {
    return stageNo > lastUnlockedStage;
}

void GameStats::saveData() {

}

void GameStats::loadData() {

}