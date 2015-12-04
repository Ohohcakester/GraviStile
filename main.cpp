#include <SFML/Graphics.hpp>
int RES_X = 800;
int RES_Y = 600;
int WIDTH = RES_X;
int HEIGHT = RES_Y;

sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "SFML works!");

class IGameObject {
public:
    int x;
    int y;

    virtual ~IGameObject() {}
    virtual void draw() = 0;
    virtual void update() = 0;

    void drawCircle(sf::CircleShape shape, int px, int py) {
        shape.setOrigin(-px+shape.getRadius(),-py+shape.getRadius());
        window.draw(shape);
    }
};

class Player : public IGameObject {
public:
    sf::CircleShape shape;

    Player() {
        x = WIDTH/2; y = HEIGHT/2;
        shape = sf::CircleShape(50);
        shape.setFillColor(sf::Color::Green);
    }

    virtual void draw() {
        drawCircle(shape, x, y);
    }

    virtual void update() {

    }

};

class GameGlobals {
public:
    Player player;
};

GameGlobals game;


void initialiseGame() {
    game = GameGlobals();
    game.player = Player();
}


void drawGameFrame() {
    game.player.draw();
}

int main() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        drawGameFrame();
        window.display();
    }

    return 0;
}