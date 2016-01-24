#ifndef EDITABLE_LEVEL_TEMPLATE_H_
#define EDITABLE_LEVEL_TEMPLATE_H_

#include "Stage.h"
class GameStage;

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

        GameStage generateStage();

        void generateCode();
    };
}

#endif