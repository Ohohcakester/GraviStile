#include <SFML/Graphics.hpp>
#include <iostream>
int RES_X = 800;
int RES_Y = 600;
int WIDTH = RES_X;
int HEIGHT = RES_Y;

sf::RenderWindow window(sf::VideoMode(RES_X, RES_Y), "SFML works!");

class Keyboard {
public:
    bool up,down,left,right;

    Keyboard() {
    }

    void update() {
        up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    }
};

class IGameObject {
public:
    int x;
    int y;

    virtual ~IGameObject() {}
    virtual void draw() = 0;
    virtual void update(Keyboard k) = 0;

    void drawCircle(sf::CircleShape shape, int px, int py) {
        shape.setOrigin(-px+shape.getRadius(),-py+shape.getRadius());
        window.draw(shape);
    }
};

class Player : public IGameObject {
public:
    sf::CircleShape shape;
    int vx;
    int vy;
    int speed;

    Player() {
        x = WIDTH/2; y = HEIGHT/2;
        speed = 10;
        shape = sf::CircleShape(50);
        shape.setFillColor(sf::Color::Green);
    }

    virtual void draw() {
        drawCircle(shape, x, y);
    }

    virtual void update(Keyboard k) {
        vx = 0; vy = 0;
        if (k.down) vy += speed;
        if (k.up) vy -= speed;
        if (k.left) vx -= speed;
        if (k.right) vx += speed;

        x += vx;
        y += vy;
    }

};

class GameGlobals {
public:
    Player player;
};


GameGlobals game;
Keyboard key;

void keyPress(sf::Keyboard::Key keyCode) {
    std::cout << "Press " << keyCode << std::endl;
}

void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window.close();
            break;
        }
        case sf::Event::KeyPressed: {
            keyPress(event.key.code);
        }
    }
}

void initialiseGame() {
    game = GameGlobals();
    game.player = Player();
}

void updateGame() {
    key.update();
    game.player.update(key);
}

void drawGameFrame() {
    game.player.draw();
}

int main() {
    sf::Clock clock;

    float frameTime = 1/60.f;
    float dTime = 0;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        if (dTime > frameTime) {
            dTime -= frameTime;

            updateGame();

            if (dTime < frameTime) {
                // frame skip if lagging
                window.clear();
                drawGameFrame();
                window.display();
            }
        }
    }

    return 0;
}