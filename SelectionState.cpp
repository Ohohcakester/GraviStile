#include "SelectionState.h"

using namespace editor;

void SelectionState::clear() {
    type = selection_none;
    selectedPlatform = nullptr;
    selectedSwitch = nullptr;
    selectedLaserSource = nullptr;
}

void SelectionState::selectPlatform(Platform* platform) {
    type = selection_platform;
    selectedPlatform = platform;
}

void SelectionState::selectSwitch(LaserSwitch* laserSwitch) {
    type = selection_switch;
    selectedSwitch = laserSwitch;
}

void SelectionState::selectLaserSource(LaserSource* laserSource) {
    type = selection_laserSource;
    selectedLaserSource = laserSource;
}

void SelectionState::selectPlayer() {
    type = selection_player;
}

void SelectionState::selectDoor() {
    type = selection_door;
}