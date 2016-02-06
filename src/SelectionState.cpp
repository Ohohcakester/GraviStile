#include "SelectionState.h"

using namespace editor;

void SelectionState::clear() {
    type = selection_none;
    selectedPlatform = nullptr;
    selectedSwitch = nullptr;
    selectedLaserSource = nullptr;
}

void SelectionState::selectPlatform(PlatformTemplate* platform) {
    type = selection_platform;
    selectedPlatform = platform;
}

void SelectionState::selectSwitch(LaserTargetTemplate* laserSwitch) {
    type = selection_switch;
    selectedSwitch = laserSwitch;
}

void SelectionState::selectLaserSource(LaserSourceTemplate* laserSource) {
    type = selection_laserSource;
    selectedLaserSource = laserSource;
}

void SelectionState::selectPlayer() {
    type = selection_player;
}

void SelectionState::selectDoor() {
    type = selection_door;
}