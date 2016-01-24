#ifndef SELECTION_STATE_H_
#define SELECTION_STATE_H_

class Platform;
class LaserSwitch;
class LaserSource;

namespace editor {
    enum eSelectionType {
        selection_none,
        selection_platform,
        selection_switch,
        selection_laserSource,
        selection_door,
        selection_player,
    };

    class SelectionState {
        int type;

        Platform* selectedPlatform;
        LaserSwitch* selectedSwitch;
        LaserSource* selectedLaserSource;

        void clear();
        void selectPlatform(Platform* platform);
        void selectSwitch(LaserSwitch* laserSwitch);
        void selectLaserSource(LaserSource* laserSource);
        void selectPlayer();
        void selectDoor();
        
    };
}


#endif