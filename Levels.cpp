#include "Stage.h"

/* ======================== *
 *     REGION - LEVELS      *
 * ======================== */

GameStage level1() {
    std::string name = "Gap";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 1, 1, 1, false, dir_up),
        PlatformTemplate(5, 1, 1, 1, false, dir_up),
    };

    PlayerTemplate player(0, 0, dir_up);
    DoorTemplate door(6, 0, dir_up);
    double zoom = 1.2;

    return GameStage(name, platforms, player, door, zoom);
}


GameStage level2() {
    std::string name = "Roundabout Route";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 2, false, dir_up),
        PlatformTemplate(2, 4, 0, 1, false, dir_up),
        PlatformTemplate(5, 1, 1, 1, true, dir_up),
        PlatformTemplate(5, 4, 1, 1, true, dir_left),
    };

    PlayerTemplate player(0, 0, dir_up);
    DoorTemplate door(2, 3, dir_up);
    double zoom = 1.0;

    return GameStage(name, platforms, player, door, zoom);
}


GameStage level3() {
    std::string name = "Obstruction";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(1, 2, 1, 2, true, dir_up),
        PlatformTemplate(-1, 2, 0, 0, false, dir_up),
        PlatformTemplate(4, 2, 1, 2, true, dir_left),
        PlatformTemplate(5, 0, 0, 0, true, dir_up),
        PlatformTemplate(7, 2, 0, 1, true, dir_down),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom);
}

GameStage level4() {
    std::string name = "Upside-Down Cage";

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
    double zoom = 1.0;

    return GameStage(name, platforms, player, door, zoom);
}


GameStage level5() {
    std::string name = "Synchronized";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 2, 1, 1, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate(4, 1, 1, 1, true, dir_left),
        PlatformTemplate(6, 2, 1, 1, true, dir_up)._spinConnectionIndex(1),
    };

    PlayerTemplate player(5, 1, dir_up);
    DoorTemplate door(2, 0, dir_up);
    double zoom = 1.0;

    return GameStage(name, platforms, player, door, zoom);
}

GameStage level6() {
    std::string name = "Positioning";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(0, 1, 1, 1, true, dir_right),
        PlatformTemplate::create(0, 4, 1, 1, true, dir_right),
        PlatformTemplate::create(1, 5, 0, 3, true, dir_up),
        PlatformTemplate::create(3, 4, 1, 0, false, dir_right),
        PlatformTemplate::create(6, 4, 1, 1, true, dir_up),
        PlatformTemplate::create(6, 6, 1, 1, false, dir_up),
    };

    PlayerTemplate player(1, 4, dir_up);
    DoorTemplate door(6, 7, dir_down);
    double zoom = 1.0;

    return GameStage(name, platforms, player, door, zoom);
}

GameStage level7() {
    std::string name = "Uncooperative";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(1, 3, 0, 2, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate(2, 6, 2, 0, true, dir_up),
        PlatformTemplate(2, 8, 0, 0, true, dir_right),
        PlatformTemplate::create(4, 1, 1, 0, true, dir_left)._spinConnectionIndex(1),
    };

    PlayerTemplate player(2, 2, dir_up);
    DoorTemplate door(3, 0, dir_up);
    double zoom = 0.8;

    return GameStage(name, platforms, player, door, zoom);
}


GameStage level8() {
    std::string name = "Pivots";

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
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom);
}

GameStage level9() {
    std::string name = "Bumpers";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(3, 2, 2, 1, true, dir_down)._spinConnectionIndex(2),
        PlatformTemplate::create(7, 1, 1, 2, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate::create(8, 4, 1, 2, true, dir_down)._spinConnectionIndex(1),
        PlatformTemplate::create(10, -1, 1, 1, true, dir_right)._spinConnectionIndex(2),
        PlatformTemplate::create(7, -2, 0, 1, false, dir_left),
        PlatformTemplate::create(10, -3, 0, 0, false, dir_right),
    };

    PlayerTemplate player(4, 1, dir_up);
    DoorTemplate door(11, -3, dir_right);
    double zoom = 1;

    return GameStage(name, platforms, player, door, zoom);
}

GameStage level10() {
    std::string name = "Laser Barrier";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(-1, -1, 0, 0, false, dir_up)._id(1),
        PlatformTemplate::create(10, 0, 0, 0, false, dir_up),
        PlatformTemplate::create(0, 2, 1, 3, true, dir_up),
        PlatformTemplate::create(6, 1, 1, 3, true, dir_down),
        PlatformTemplate::create(6, 3, 1, 0, true, dir_up),
        PlatformTemplate::create(9, 2, 0, 1, true, dir_up),
        PlatformTemplate::create(9, 4, 1, 1, false, dir_down),
        PlatformTemplate::create(7, 5, 1, 0, true, dir_left),
    };


    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_right, 1),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(10, -1, dir_up);
    double zoom = 1;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}

GameStage level11() {
    std::string name = "Leap of Faith";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(3, 2, 1, 1, true, dir_up),
        PlatformTemplate(0, 4, 1, 1, true, dir_right),
        PlatformTemplate(4, 3, 0, 3, true, dir_up),
        PlatformTemplate(3, 6, 0, 2, true, dir_left),
        PlatformTemplate(6, 6, 0, 1, true, dir_up),
    };

    PlayerTemplate player(3, 0, dir_up);
    DoorTemplate door(5, 7, dir_left);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom);
}

GameStage level12() {
    std::string name = "Laser Grid";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(3, 3, 3, 3, true, dir_up),
        PlatformTemplate::create(2, -3, 0, 0, false, dir_up)._id(1),
        PlatformTemplate::create(5, -3, 0, 0, false, dir_up)._id(2),
        PlatformTemplate::create(-3, 0, 0, 0, false, dir_up)._id(3),
        PlatformTemplate::create(-3, 6, 0, 0, false, dir_up)._id(4),
        PlatformTemplate::create(1, 9, 0, 0, false, dir_up)._id(5),
        PlatformTemplate::create(5, 9, 0, 0, false, dir_up)._id(6),
        PlatformTemplate::create(9, 3, 0, 0, false, dir_up)._id(7),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_down, 1),
        LaserSourceTemplate(0, dir_down, 2),
        LaserSourceTemplate(0, dir_right, 3),
        LaserSourceTemplate(0, dir_right, 4),
        LaserSourceTemplate(0, dir_up, 5),
        LaserSourceTemplate(0, dir_up, 6),
        LaserSourceTemplate(0, dir_left, 7),
    };

    PlayerTemplate player(0, 2, dir_up);
    DoorTemplate door(6, 2, dir_up);
    double zoom = 0.8;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}


GameStage level13() {
    std::string name = "Redirection";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(3, 4, 2, 1, true, dir_up),
        PlatformTemplate::create(5, -1, 1, 1, true, dir_down)._id(1),
        PlatformTemplate::create(7, -2, 0, 1, false, dir_left),
        PlatformTemplate::create(7, 3, 1, 3, false, dir_up),
        PlatformTemplate::create(7, 5, 1, 0, true, dir_down),
        PlatformTemplate::create(7, 7, 1, 1, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate::create(2, 7, 1, 0, true, dir_left)._spinConnectionIndex(1),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_up, 1),
    };

    PlayerTemplate player(3, 3, dir_up);
    DoorTemplate door(8, 2, dir_up);
    double zoom = 0.8;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}

GameStage level14() {
    std::string name = "Criss-Cross";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(4, 0, 1, 1, true, dir_up)._id(5),
        PlatformTemplate::create(6, 0, 0, 1, true, dir_right),
        PlatformTemplate::create(9, 0, 1, 1, true, dir_up),
        PlatformTemplate::create(5, 4, 1, 1, true, dir_up)._id(1),
        PlatformTemplate::create(1, 4, 1, 1, true, dir_down)._id(2),
        PlatformTemplate::create(9, 4, 1, 1, true, dir_up)._id(6),
        PlatformTemplate::create(0, 1, 1, 1, true, dir_right)._id(3),
        PlatformTemplate::create(0, -3, 1, 0, true, dir_left)._id(4),
    };


    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_up, 1),
        LaserSourceTemplate(0, dir_left, 2),
        LaserSourceTemplate(0, dir_right, 2),
        LaserSourceTemplate(0, dir_up, 3),
        LaserSourceTemplate(0, dir_down, 3),
        LaserSourceTemplate(0, dir_down, 4),
        LaserSourceTemplate(1, dir_down, 5),
        LaserSourceTemplate(0, dir_left, 6),
        LaserSourceTemplate(0, dir_right, 6),
    };

    PlayerTemplate player(4, -1, dir_up);
    DoorTemplate door(-1, -3, dir_left);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}


GameStage level15() {
    std::string name = "Stoppered";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(4, 3, 1, 1, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate::create(1, 0, 2, 0, true, dir_right)._spinConnectionIndex(1),
        PlatformTemplate::create(7, 0, 0, 2, true, dir_left)._spinConnectionIndex(1),
        PlatformTemplate::create(0, 3, 1, 1, true, dir_right),
        PlatformTemplate::create(8, 3, 1, 1, true, dir_left),
        PlatformTemplate::create(-7, 5, 0, 0, false, dir_up)._id(1),
        PlatformTemplate::create(15, 5, 0, 0, false, dir_up)._id(2),
        PlatformTemplate::create(0, -3, 1, 0, true, dir_down),
        PlatformTemplate::create(8, -3, 0, 1, true, dir_down),
    };


    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_right, 1),
        LaserSourceTemplate(0, dir_left, 2),
    };

    PlayerTemplate player(4, 2, dir_up);
    DoorTemplate door(4, 4, dir_down);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}

GameStage level16() {
    std::string name = "Mechanical Lock";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 2, 1, 1, true, dir_up),
        PlatformTemplate(4, 1, 0, 0, true, dir_right),
        PlatformTemplate::create(3, 5, 2, 2, true, dir_up)._id(1),
        PlatformTemplate(6, 6, 1, 1, true, dir_right),
        PlatformTemplate(5, 8, 1, 1, true, dir_down),
        PlatformTemplate(8, 7, 1, 1, true, dir_up),
        PlatformTemplate(3, 9, 0, 0, true, dir_left),
        PlatformTemplate(5, 11, 1, 1, true, dir_right),
        PlatformTemplate(7, 9, 0, 0, false, dir_up)._id(2),
        PlatformTemplate(7, 0, 1, 1, true, dir_down),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_left, 1),
        LaserSourceTemplate(0, dir_right, 1),
        LaserSourceTemplate(0, dir_right, 2),
    };

    PlayerTemplate player(3, 4, dir_up);
    DoorTemplate door(6, 12, dir_right);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}


GameStage level17() {
    std::string name = "Laser Switch";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(3, 0, 1, 1, true, dir_up)._platformSwitchConnectionIndex(1),
        PlatformTemplate::create(1, 2, 1, 2, false, dir_left)._id(1),
        PlatformTemplate::create(5, 2, 1, 2, false, dir_left)._id(2),
        PlatformTemplate::create(3, 2, 1, 1, false, dir_up),
        PlatformTemplate::create(0, 0, 1, 0, true, dir_up),
        PlatformTemplate::create(6, 0, 0, 1, true, dir_up),
        PlatformTemplate::create(0, 3, 0, 1, true, dir_down),
        PlatformTemplate::create(6, 3, 1, 0, true, dir_down),
        PlatformTemplate::create(3, 4, 1, 1, true, dir_left),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(-1, dir_up, 2),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(-1, dir_down, 1, 1),
    };

    PlayerTemplate player(2, -1, dir_up);
    DoorTemplate door(3, 1, dir_up);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}


GameStage level18() {
    std::string name = "Puzzle Box";

    std::vector<PlatformTemplate> platforms{
        // Starting Platform
        PlatformTemplate::create(7, 5, 1, 1, true, dir_up)._id(1),

        // Edge lasers
        PlatformTemplate::create(7, -4, 2, 2, false, dir_up)._id(2),
        PlatformTemplate::create(7, 14, 2, 2, false, dir_up)._id(3),
        PlatformTemplate::create(5, 12, 0, 0, false, dir_up)._platformSwitchConnectionIndex(11),
        PlatformTemplate::create(9, -2, 0, 0, false, dir_up)._platformSwitchConnectionIndex(12),
        
        // Targets
        PlatformTemplate::create(3, 6, 1, 1, true, dir_up)._id(4),
        PlatformTemplate::create(11, 4, 1, 1, true, dir_up)._id(5),

        // Switch Platforms
        PlatformTemplate::create(7, 0, 1, 1, true, dir_left)._id(6),
        PlatformTemplate::create(7, 10, 1, 1, true, dir_right)._id(7),

        // Other Platforms
        PlatformTemplate::create(3, 3, 1, 1, true, dir_down),
        PlatformTemplate::create(11, 0, 1, 1, true, dir_left),
        PlatformTemplate::create(11, 8, 1, 1, true, dir_right),

        // Goal Gate
        PlatformTemplate::create(15, 4, 1, 1, true, dir_up),

        PlatformTemplate::create(16, 1, 2, 1, false, dir_up),
        PlatformTemplate::create(16, 7, 2, 1, false, dir_up),
        PlatformTemplate::create(17, 4, 2, 2, false, dir_right),

        PlatformTemplate::create(14, 2, 0, 1, false, dir_right)._platformSwitchConnectionIndex(20),
        PlatformTemplate::create(14, 5, 0, 1, false, dir_right)._platformSwitchConnectionIndex(20),
        PlatformTemplate::create(15, 3, 1, 0, false, dir_right)._platformSwitchConnectionIndex(21),
        PlatformTemplate::create(15, 6, 1, 0, false, dir_right)._platformSwitchConnectionIndex(21),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(-1, dir_up, 1),
        LaserSourceTemplate(1, dir_down, 1),
        LaserSourceTemplate(2, dir_down, 2),
        LaserSourceTemplate(-2, dir_up, 3),
        LaserSourceTemplate(0, dir_up, 6),
        LaserSourceTemplate(0, dir_up, 7),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(0, dir_right, 4, 11),
        LaserTargetTemplate(0, dir_left, 5, 12),

        LaserTargetTemplate(0, dir_down, 2, 20),
        LaserTargetTemplate(0, dir_up, 3, 21),
    };

    PlayerTemplate player(7, 4, dir_up);
    DoorTemplate door(16, 5, dir_down);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}


GameStage level19() {
    std::string name = "Twist and Turn";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(1, 11, 1, 1, true, dir_up),
        PlatformTemplate::create(0, 8, 1, 2, true, dir_right),
        PlatformTemplate::create(1, 6, 1, 1, true, dir_up),
        PlatformTemplate::create(2, 4, 1, 1, true, dir_right),
        PlatformTemplate::create(3, 3, 0, 0, true, dir_up),
        PlatformTemplate::create(4, 1, 1, 1, true, dir_right),
    };

    PlayerTemplate player(0, 5, dir_up);
    DoorTemplate door(1, 7, dir_down);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom);
}


GameStage level20() {
    std::string name = "Coupled";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(4, -4, 3, 2, false, dir_up)._id(1),
        PlatformTemplate::create(4, 8, 1, 2, false, dir_up)._id(2),
        PlatformTemplate::create(6, 6, 0, 0, false, dir_up)._platformSwitchConnectionIndex(1)._isDisabled(true),
        PlatformTemplate::create(1, 1, 1, 1, true, dir_up),
        PlatformTemplate::create(2, 4, 1, 1, true, dir_left),
        PlatformTemplate::create(4, 1, 1, 1, true, dir_left),
        PlatformTemplate::create(5, 4, 1, 1, true, dir_left),
        PlatformTemplate::create(0, 4, 1, 0, true, dir_up),
        PlatformTemplate::create(-2, 1, 1, 0, true, dir_up),
        PlatformTemplate::create(7, 4, 1, 0, false, dir_right),
        PlatformTemplate::create(1, -1, 0, 2, false, dir_left),
        PlatformTemplate::create(5, -2, 1, 1, true, dir_right)._platformSwitchConnectionIndex(2)._isDisabled(true),
        PlatformTemplate::create(8, 2, 1, 1, true, dir_right)._id(3),
        PlatformTemplate::create(10, 1, 1, 1, true, dir_up)._id(4),

    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(2, dir_up, 2),
        LaserSourceTemplate(-1, dir_up, 2),
        LaserSourceTemplate(1, dir_down, 4),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(-1, dir_down, 1, 1),
        LaserTargetTemplate(0, dir_right, 3, 2),
    };

    PlayerTemplate player(0, -1, dir_up);
    DoorTemplate door(7, 5, dir_down);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}


GameStage level21() {
    std::string name = "Disarm";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(3, 0, 0, 4, false, dir_up),
        PlatformTemplate(3, 3, 2, 0, true, dir_right),
        PlatformTemplate(5, 2, 1, 1, true, dir_right),
        PlatformTemplate(5, 4, 2, 1, false, dir_up)._id(1),
        PlatformTemplate(7, 4, 0, 3, true, dir_up)._id(2),
        //PlatformTemplate(13, 2, 1, 1, true, dir_down),
        PlatformTemplate(15, 5, 1, 1, true, dir_right),
        PlatformTemplate(18, 8, 1, 1, true, dir_left),
        PlatformTemplate(13, 9, 1, 1, true, dir_up),
        PlatformTemplate(11, 12, 1, 1, true, dir_up),
        //PlatformTemplate(7, 13, 1, 1, true, dir_right),
        PlatformTemplate(6, 6, 1, 2, false, dir_right)._id(3),
        PlatformTemplate(2, 14, 1, 1, true, dir_up),
        PlatformTemplate(3, 12, 1, 1, true, dir_down),
        PlatformTemplate(5, 10, 1, 1, true, dir_down),
        PlatformTemplate(4, 6, 1, 1, false, dir_right),
        PlatformTemplate(4, 8, 0, 1, true, dir_down),

        PlatformTemplate(5, 5, 0, 0, false, dir_down)._platformSwitchConnectionIndex(1),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_down, 1),
        LaserSourceTemplate(2, dir_down, 2),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(0, dir_up, 3, 1),
    };


    PlayerTemplate player(8, 2, dir_up);
    DoorTemplate door(5, 6, dir_right);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}


GameStage level22() {
    std::string name = "Deadlock";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(2, 1, 0, 0, false, dir_down),
        PlatformTemplate(3, 2, 1, 2, true, dir_right),
        PlatformTemplate(5, 1, 1, 1, true, dir_up),
        PlatformTemplate(1, 5, 1, 0, true, dir_down),
        PlatformTemplate(5, 4, 1, 1, true, dir_left),
        PlatformTemplate(7, 3, 1, 1, true, dir_up),
        PlatformTemplate(9, 2, 0, 2, true, dir_right),
        PlatformTemplate(11, 5, 0, 2, true, dir_left),
        PlatformTemplate(12, 6, 1, 0, false, dir_up),
        PlatformTemplate(9, 6, 2, 1, true, dir_up),
        PlatformTemplate(1, 8, 0, 2, true, dir_left),
        PlatformTemplate(4, 7, 1, 2, true, dir_up),
        PlatformTemplate(4, 9, 0, 1, false, dir_down),
        PlatformTemplate(6, 10, 2, 0, true, dir_right),
        PlatformTemplate(8, 10, 0, 0, true, dir_left),
        PlatformTemplate(10, 1, 0, 0, true, dir_up),
    };

    PlayerTemplate player(5, 0, dir_up);
    DoorTemplate door(11, 1, dir_up);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom);
}



GameStage level23() {
    std::string name = "Reconfigure";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(3, 2, 2, 1, true, dir_up)._id(11),
        PlatformTemplate::create(7, 2, 2, 1, true, dir_down)._id(10),
        PlatformTemplate::create(4, 6, 1, 1, true, dir_left),
        PlatformTemplate::create(7, 5, 1, 1, true, dir_up),

        PlatformTemplate::create(6, 9, 1, 1, true, dir_right),
        PlatformTemplate::create(8, 11, 0, 0, true, dir_up),

        PlatformTemplate::create(2, 10, 1, 3, true, dir_right),
        PlatformTemplate::create(0, 4, 1, 1, true, dir_right)._spinConnectionIndex(1),
        PlatformTemplate::create(0, 7, 0, 1, true, dir_down)._spinConnectionIndex(1),

        // Goal Platform
        PlatformTemplate::create(10, 2, 0, 1, false, dir_up),

        // Laser Platforms
        PlatformTemplate::create(1, -6, 0, 0, false, dir_up)._id(1),
        PlatformTemplate::create(5, 15, 0, 0, false, dir_up)._id(3),
        PlatformTemplate::create(9, 15, 0, 0, false, dir_up)._id(4),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_down, 1),
        LaserSourceTemplate(0, dir_up, 3),
        LaserSourceTemplate(0, dir_up, 4),
    };
    
    PlayerTemplate player(3, 1, dir_up);
    DoorTemplate door(11, 1, dir_up);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom, laserSources);
}



GameStage level24() {
    std::string name = "Cipher Machine";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(3, 3, 1, 1, true, dir_down)._spinConnectionIndex(1),
        PlatformTemplate::create(6, 3, 1, 1, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate::create(3, 6, 1, 1, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate::create(6, 6, 1, 1, true, dir_down)._spinConnectionIndex(1),

        PlatformTemplate::create(9, 1, 1, 1, true, dir_down),
        PlatformTemplate::create(10, 3, 1, 1, true, dir_up)._id(1),
        PlatformTemplate::create(10, 6, 1, 1, true, dir_down)._id(10),
    };

    std::vector<LaserSourceTemplate> laserSources{
        LaserSourceTemplate(0, dir_up, 1),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(0, dir_up, 10, 1),
    };


    PlayerTemplate player(3, 2, dir_up);
    DoorTemplate door(11, 1, dir_up);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}



/* ======================= *
 *  REGION - LEVELS - END  *
 * ======================= */




/* ============================== *
 *  REGION - TEST STAGES - START  *
 * ============================== */


GameStage testStage1() {
    std::string name = "Test 1";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate::create(1, 2, 1, 2, true, dir_up)._id(1),
        PlatformTemplate::create(4, 2, 1, 2, true, dir_left)._spinConnectionIndex(1),
        PlatformTemplate::create(5, 0, 0, 0, true, dir_up)._spinConnectionIndex(1),
        PlatformTemplate::create(7, 2, 1, 0, true, dir_down)._spinConnectionIndex(2),
        PlatformTemplate::create(0, 6, 2, 1, true, dir_right)._spinConnectionIndex(2)._id(2),
        PlatformTemplate::create(1, -2, 1, 1, true, dir_left)._pivotSwitchConnectionIndex(1),
        PlatformTemplate::create(1, -6, 1, 1, true, dir_left)._platformSwitchConnectionIndex(1),
        PlatformTemplate::create(-2, 0, 1, 1, true, dir_right)._platformSwitchConnectionIndex(1)._isDisabled(true),
        PlatformTemplate::create(-4, 2, 1, 1, true, dir_left)._id(3),
        PlatformTemplate::create(-2, 4, 1, 3, false, dir_right)._platformSwitchConnectionIndex(2),
    };

    std::vector<LaserSourceTemplate> laserSources{
        //LaserSourceTemplate(0, dir_right, 1),
        LaserSourceTemplate(-1, dir_down, 1),
        LaserSourceTemplate(-1, dir_down, 3),
    };

    std::vector<LaserTargetTemplate> laserTargets{
        LaserTargetTemplate(1, dir_down, 1, 1),
        LaserTargetTemplate(1, dir_down, 3, 2),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    double zoom = 0.9;

    return GameStage(name, platforms, player, door, zoom, laserSources, laserTargets);
}


GameStage testStage2() {
    std::string name = "Test 2";

    std::vector<PlatformTemplate> platforms{
        PlatformTemplate(-2, 2, 1, 3, true, dir_up, 1, 2),
        PlatformTemplate(5, -1, 3, 1, true, dir_up, 1, 1),
        PlatformTemplate(5, 2, 0, 0, true, dir_up, 1, 1),
        PlatformTemplate(-3, 5, 3, 3, true, dir_up, 1, 1),


        PlatformTemplate::create(3, 12, 1, 1, true, dir_up),
        PlatformTemplate::create(1, 10, 1, 1, true, dir_left),
        PlatformTemplate::create(1, 7, 1, 1, true, dir_left),
        PlatformTemplate::create(1, 4, 1, 1, true, dir_left),
    };

    PlayerTemplate player(0, 1, dir_up);
    DoorTemplate door(7, 1, dir_up);
    double zoom = 0.7;

    return GameStage(name, platforms, player, door, zoom);
}

/* ============================ *
 *  REGION - TEST STAGES - END  *
 * ============================ */

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
    case 11: return level11();
    case 12: return level12();
    case 13: return level13();
    case 14: return level14();
    case 15: return level15();
    case 16: return level16();
    case 17: return level17();
    case 18: return level18();
    case 19: return level19();
    case 20: return level20();
    case 21: return level21();
    case 22: return level22();
    case 23: return level23();
    case 24: return level24();
    case 100: return testStage1();
    case 101: return testStage2();
    }

    return level1();
}