#ifndef EDITABLE_LEVEL_TEMPLATE_H_
#define EDITABLE_LEVEL_TEMPLATE_H_

#include "Stage.h"
struct GameStage;

namespace editor {
    class EditableLevelTemplate {
    public:
        std::vector<PlatformTemplate> platforms;
        std::vector<LaserSourceTemplate> laserSources;
        std::vector<LaserTargetTemplate> laserTargets;
        PlayerTemplate player;
        DoorTemplate door;
        std::string name;
        float zoom;
        
        EditableLevelTemplate();
        EditableLevelTemplate(GameStage loadFrom);

        void remove(PlatformTemplate* platform);
        void remove(LaserSourceTemplate* laserSource);
        void remove(LaserTargetTemplate* laserTarget);

        void removeAttached(int platformId);

        GameStage generateStage();

        void generateCode();
    };
}

#endif