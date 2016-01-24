#ifndef SELECTION_STATE_H_
#define SELECTION_STATE_H_

#include "Stage.h"

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
    public:
        int type;

        PlatformTemplate* selectedPlatform;
        LaserTargetTemplate* selectedSwitch;
        LaserSourceTemplate* selectedLaserSource;

        void clear();
        void selectPlatform(PlatformTemplate* platform);
        void selectSwitch(LaserTargetTemplate* laserSwitch);
        void selectLaserSource(LaserSourceTemplate* laserSource);
        void selectPlayer();
        void selectDoor();
        
    };
}


#endif