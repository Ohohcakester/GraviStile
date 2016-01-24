#include "EditableLevelTemplate.h"
#include "Stage.h"
#include "gameobjects.h"
#include "FileIO.h"
#include "Orientations.h"

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

void EditableLevelTemplate::remove(PlatformTemplate* platform) {
    size_t index = -1;
    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        if (&platforms[i] == platform) {
            index = i;
            break;
        }
    }

    if (index == -1) return; // not present.
    removeAttached(platform->id);
    platforms.erase(platforms.begin() + index);
}

void EditableLevelTemplate::remove(LaserSourceTemplate* laserSource) {
    size_t index = -1;
    for (size_t i = 0, n = laserSources.size(); i < n; ++i) {
        if (&laserSources[i] == laserSource) {
            index = i;
            break;
        }
    }

    if (index == -1) return; // not present.
    laserSources.erase(laserSources.begin() + index);
}

void EditableLevelTemplate::remove(LaserTargetTemplate* laserTarget) {
    size_t index = -1;
    for (size_t i = 0, n = laserTargets.size(); i < n; ++i) {
        if (&laserTargets[i] == laserTarget) {
            index = i;
            break;
        }
    }

    if (index == -1) return; // not present.
    laserTargets.erase(laserTargets.begin() + index);
}

void EditableLevelTemplate::removeAttached(int platformId) {
    if (platformId == -1) return;

    for (size_t i = 0; i < laserSources.size(); ++i) {
        if (laserSources[i].platformId == platformId) {
            laserSources.erase(laserSources.begin() + i);
            i--;
        }
    }

    for (size_t i = 0; i < laserTargets.size(); ++i) {
        if (laserTargets[i].platformId == platformId) {
            laserTargets.erase(laserTargets.begin() + i);
            i--;
        }
    }
}

GameStage EditableLevelTemplate::generateStage() {
    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}

std::string strBool(bool value) {
    return value ? "true" : "false";
}

void EditableLevelTemplate::generateCode() {
    FileIO fileIO("outputLevel.txt");

    std::ofstream& s = fileIO.stream;

    s << "std::string name = \"" << name << "\";\n";
    s << "\n";
    s << "std::vector<PlatformTemplate> platforms{\n";

    for (size_t i = 0, n = platforms.size(); i < n; ++i) {
        PlatformTemplate& p = platforms[i];
        s << "    PlatformTemplate(" << p.pivotX << ", " << p.pivotY << ", "
            << p.leftTiles << ", " << p.rightTiles << ", " << strBool(p.rotatable)
            << ", " << orientationName(p.orientation) << ")";

        if (p.id != -1) {
            s << "._id(" << p.id << ")";
        }
        if (p.isDisabled) {
            s << "._isDisabled(true)";
        }
        if (p.spinConnectionIndex != -1) {
            s << "._spinConnectionIndex(" << p.spinConnectionIndex << ")";
        }
        if (p.pivotSwitchConnectionIndex != -1) {
            s << "._pivotSwitchConnectionIndex(" << p.pivotSwitchConnectionIndex << ")";
        }
        if (p.platformSwitchConnectionIndex != -1) {
            s << "._platformSwitchConnectionIndex(" << p.platformSwitchConnectionIndex << ")";
        }

        s << ",\n";
    }
    s << "};\n";

    s << "\n";

    s << "std::vector<LaserSourceTemplate> laserSources{";
    for (size_t i = 0, n = laserSources.size(); i < n; ++i) {
        LaserSourceTemplate& l = laserSources[i];
        s << "    LaserSourceTemplate(" << l.position << ", " <<
            orientationName(l.orientation) << ", " << l.platformId << "),\n";
    }
    s << "};\n";

    s << "\n";

    s << "std::vector<LaserTargetTemplate> laserTargets{";
    for (size_t i = 0, n = laserTargets.size(); i < n; ++i) {
        LaserTargetTemplate& l = laserTargets[i];
        s << "    LaserTargetTemplate(" << l.position << ", " <<
            orientationName(l.orientation) << ", " << l.platformId << 
            ", " << l.switchConnectionIndex << "),\n";
    }
    s << "};\n";

    s << "\n";

    s << "PlayerTemplate player(" << player.x << ", " << player.y << ", " << orientationName(player.orientation) << ");\n";
    s << "DoorTemplate door(" << door.x << ", " << door.y << ", " << orientationName(door.orientation) << ");\n";
    s << "double zoom = " << zoom << ";\n";

    s << "\n";
    s << "return GameStage(name, platforms, player, door, zoom, laserSources);\n";

    fileIO.close();
}