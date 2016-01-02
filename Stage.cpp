#include <iostream>
#include "globals.h"
#include "Stage.h"
#include "SpinConnection.h"

void linkPlatforms(GameStage* gameStage, std::vector<Platform*>* _platforms, std::vector<SpinConnection*>* spinConnections) {
    std::vector<Platform*>& platforms = *_platforms;

    std::map<int, std::vector<Platform*>> platformGroups;
    int nPlatforms = gameStage->platforms.size();
    for (int i = 0; i < nPlatforms; ++i) {
        int index = gameStage->platforms[i].spinConnectionIndex;
        if (index == -1) continue;
        auto vec = platformGroups.find(index);
        if (vec == platformGroups.end()) {
            platformGroups.emplace(index, std::vector<Platform*> {platforms[i]});
        }
        else {
            vec->second.push_back(platforms[i]);
        }
    }

    // link.
    for (auto itr = platformGroups.begin(); itr != platformGroups.end(); ++itr) {
        spinConnections->push_back(createSpinConnection(itr->second));
    }
}

void initialiseFromStageObject(GameStage gameStage) {
    game.cleanup();

    int nPlatforms = gameStage.platforms.size();
    std::vector<Platform*> platforms(nPlatforms);
    for (int i = 0; i < nPlatforms; ++i) {
        PlatformTemplate* t = &gameStage.platforms[i];
        Platform* platform = new Platform(t->pivotX, t->pivotY, t->leftTiles, t->rightTiles, t->rotatable, t->orientation);
        platforms[i] = platform;
    }
    game.platforms.swap(platforms);

    game.nTilesX = gameStage.nTilesX;
    game.nTilesY = gameStage.nTilesY;
    game.player = Player(gameStage.player.x, gameStage.player.y);
    game.player.setOrientation(game.player.orientation);
    game.zoom = gameStage.zoom;
    Door door = Door(gameStage.door.x, gameStage.door.y, gameStage.door.orientation);
    game.door = door;

    linkPlatforms(&gameStage, &game.platforms, &game.spinConnections);
}


/* ======================== *
*     REGION - LEVELS      *
* ======================== */


GameStage level1() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 1, 1, 1, false, dir_up),
        PlatformTemplate(5, 1, 1, 1, false, dir_up),
    };

    PlayerTemplate player(0, 0, dir_up);
    DoorTemplate door(6, 0, dir_up);
    int nTilesX = 7;
    int nTilesY = 3;
    double zoom = 1.2;

    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level2() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 2, false, dir_up),
        PlatformTemplate(2, 4, 0, 1, false, dir_up),
        PlatformTemplate(5, 1, 1, 1, true, dir_up),
        PlatformTemplate(5, 4, 1, 1, true, dir_left),
    };

    PlayerTemplate player(0, 0, dir_up);
    DoorTemplate door(2, 3, dir_up);
    int nTilesX = 7;
    int nTilesY = 6;
    double zoom = 1.0;

    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level3() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 2, true, dir_up),
        PlatformTemplate(4, 2, 1, 2, true, dir_left,1),
        PlatformTemplate(5, 0, 0, 0, true, dir_up,1),
        PlatformTemplate(7, 2, 1, 0, true, dir_down,2),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    int nTilesX = 9;
    int nTilesY = 4;
    double zoom = 0.9;

    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level4() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 0, 1, 1, false, dir_up),
        PlatformTemplate(2, 2, 1, 1, true, dir_up),
        PlatformTemplate(0, 4, 1, 1, true, dir_right),
        PlatformTemplate(4, 4, 1, 1, true, dir_left),
        PlatformTemplate(0, 0, 0, 0, true, dir_down),
        PlatformTemplate(4, 0, 0, 0, true, dir_down),
    };

    PlayerTemplate player(1, 1, dir_up);
    DoorTemplate door(2, 1, dir_down);
    int nTilesX = 5;
    int nTilesY = 6;
    double zoom = 1.0;
    
    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}



GameStage level5() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 3, 0, 2, true, dir_up),
        PlatformTemplate(2, 6, 1, 0, true, dir_up),
        PlatformTemplate(2, 8, 0, 0, true, dir_right),
        PlatformTemplate(4, 1, 1, 0, true, dir_left),
    };

    PlayerTemplate player(2, 2, dir_up);
    DoorTemplate door(3, 0, dir_up);
    int nTilesX = 5;
    int nTilesY = 9;
    double zoom = 0.8;

    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level6() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(3, 1, 0, 0, true, dir_right),
        PlatformTemplate(3, 4, 0, 0, true, dir_up),
        PlatformTemplate(4, 3, 0, 0, true, dir_down),
        PlatformTemplate(5, 2, 0, 0, true, dir_up),
        PlatformTemplate(5, 5, 0, 0, true, dir_down),
        PlatformTemplate(2, 6, 0, 2, true, dir_right),
        PlatformTemplate(1, 5, 0, 0, true, dir_right),
    };

    PlayerTemplate player(3, 0, dir_up);
    DoorTemplate door(3, 7, dir_down);
    int nTilesX = 8;
    int nTilesY = 9;
    double zoom = 0.9;
    
    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}

GameStage level7() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(3, 2, 1, 1, true, dir_up),
        PlatformTemplate(0, 4, 1, 1, true, dir_right),
        PlatformTemplate(4, 3, 0, 3, true, dir_up),
        PlatformTemplate(3, 6, 0, 2, true, dir_left),
        PlatformTemplate(6, 6, 0, 1, true, dir_up),
    };

    PlayerTemplate player(3, 0, dir_up);
    DoorTemplate door(5, 7, dir_left);
    int nTilesX = 8;
    int nTilesY = 8;
    double zoom = 0.9;

    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level8() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 2, 1, 1, true, dir_up),
        PlatformTemplate(4, 1, 0, 0, true, dir_right),
        PlatformTemplate(3, 5, 2, 2, true, dir_up),
        PlatformTemplate(6, 6, 1, 1, true, dir_right),
        PlatformTemplate(5, 8, 1, 1, true, dir_down),
        PlatformTemplate(8, 7, 1, 1, true, dir_up),
        PlatformTemplate(3, 9, 0, 0, true, dir_left),
        PlatformTemplate(5, 11, 1, 1, true, dir_right),
        PlatformTemplate(9, 9, 2, 2, false, dir_up),
        PlatformTemplate(7, 0, 1, 1, true, dir_down),
    };

    PlayerTemplate player(3, 4, dir_up);
    DoorTemplate door(6, 12, dir_right);
    int nTilesX = 12;
    int nTilesY = 12;
    double zoom = 0.7;
    
    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level9() {
    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 3, 1, 1, true, dir_right),
        PlatformTemplate(5, 2, 1, 1, true, dir_right),
        PlatformTemplate(5, 4, 1, 1, false, dir_up),
        PlatformTemplate(7, 4, 0, 3, true, dir_up),
        PlatformTemplate(13, 2, 1, 1, true, dir_down),
        PlatformTemplate(15, 4, 1, 1, true, dir_right),
        PlatformTemplate(18, 8, 1, 1, true, dir_right),
        PlatformTemplate(13, 9, 1, 1, true, dir_up),
        PlatformTemplate(11, 12, 1, 1, true, dir_up),
        PlatformTemplate(7, 13, 1, 1, true, dir_left),
        PlatformTemplate(6, 6, 2, 2, false, dir_right),
        PlatformTemplate(2, 14, 1, 1, true, dir_up),
        PlatformTemplate(3, 12, 1, 1, true, dir_down),
        PlatformTemplate(5, 10, 1, 1, true, dir_down),
        PlatformTemplate(4, 6, 1, 1, false, dir_right),
        PlatformTemplate(4, 8, 0, 1, true, dir_down)
    };

    PlayerTemplate player(8, 2, dir_up);
    DoorTemplate door(5, 6, dir_right);
    int nTilesX = 20;
    int nTilesY = 17;
    double zoom = 0.7;
    
    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}


GameStage level10() {
    std::vector<PlatformTemplate> platforms {
        PlatformTemplate(2, 1, 0, 0, true, dir_down),
        PlatformTemplate(3, 2, 1, 1, true, dir_right),
        PlatformTemplate(5, 1, 1, 1, true, dir_up),
        PlatformTemplate(1, 5, 2, 0, true, dir_down),
        PlatformTemplate(5, 4, 1, 1, true, dir_left),
        PlatformTemplate(7, 3, 1, 1, true, dir_up),
        PlatformTemplate(9, 2, 0, 2, true, dir_right),
        PlatformTemplate(11, 5, 0, 2, true, dir_left),
        PlatformTemplate(12, 6, 0, 0, false, dir_left),
        PlatformTemplate(9, 6, 2, 0, true, dir_up),
        PlatformTemplate(1, 8, 0, 1, true, dir_left),
        PlatformTemplate(5, 7, 1, 1, true, dir_up),
        PlatformTemplate(3, 9, 1, 1, true, dir_down),
        PlatformTemplate(6, 10, 2, 0, true, dir_right),
        PlatformTemplate(8, 10, 0, 0, true, dir_left),
        PlatformTemplate(10, 1, 0, 0, true, dir_up),
    };

    PlayerTemplate player(5, 0, dir_up);
    DoorTemplate door(11, 1, dir_up);
    int nTilesX = 13;
    int nTilesY = 12;
    double zoom = 0.9;

    return GameStage(nTilesX, nTilesY, platforms, player, door, zoom);
}



/* ======================== *
*   REGION - LEVELS - END  *
* ======================== */

GameStage getStage(int stage) {
    switch (stage) {
    case 1: return level1();
    case 2: return level2();
    case 3: return level3();
    case 4: return level4();
    case 5: return level5();
    case 6: return level6();
    case 7: return level7();
    case 8: return level8();
    case 9: return level9();
    case 10: return level10();
    }
}


void initialiseStage(int stage) {
    game.currentStage = stage;
    initialiseFromStageObject(getStage(stage));
}