#ifndef SFX_H_
#define SFX_H_

#include "BaseSfx.h"
#include <string>
class Keyboard;

namespace sfx {
    class LevelName : public Sfx {
    public:
        LevelName(std::string name, int level);

        void draw();
        void sfxUpdate();

    protected:
        std::string text;
        std::string levelNumberText;
        sf::Color color;
        sf::Text sfLevelText;
        sf::Text sfText;
    };

    class Text : public Sfx {
    public:
        Text(int x, int y, std::string name, int timeout) : Sfx(x, y), text(name), timeout(timeout) {}

        void draw();
        void sfxUpdate();

    protected:
        std::string text;
        int timeout;
    };
}


#endif