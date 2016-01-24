#include "EditableLevelTemplate.h"
#include "Stage.h"
#include "gameobjects.h"

using namespace editor;

EditableLevelTemplate::EditableLevelTemplate() :
    player(0, 0, dir_up),
    door(0, 0, dir_up),
    name("Test Stage"),
    zoom(0.9f)
{

}

EditableLevelTemplate::EditableLevelTemplate(GameStage loadFrom) :
    platforms(loadFrom.platforms),
    laserSources(loadFrom.laserSources),
    laserTargets(loadFrom.laserTargets),
    player(loadFrom.player),
    door(loadFrom.door),
    name(loadFrom.name),
    zoom(loadFrom.zoom)
{

}


GameStage EditableLevelTemplate::generateStage() {
    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}