#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <random>

class Pong {
    sf::SoundBuffer sounds[3];
    sf::Sound toPlay;
    sf::RectangleShape rackets[2];
    sf::CircleShape aBall;
    sf::Vector2f ballSpeed;
    std::minstd_rand generate;
    int result[2];
    sf::Vector2f racketSpeed;
    float windowX, windowY;

public:
    Pong(float winX, float winY): windowX(winX), windowY(winY) {}

    void setSounds() {
        sounds[0].loadFromFile("./ping_pong_8bit_plop.wav");
        sounds[1].loadFromFile("./ping_pong_8bit_beeep.wav");
        sounds[2].loadFromFile("./ping_pong_8bit_peeeeeep.wav");
    }

    void init(float ballR, float racketW, float racketH) {
        result[0] = 0;
        result[1] = 0;

        racketSpeed = sf::Vector2f(0.f, 6.f);

        ballSpeed.y = 0;
        ballSpeed.x = (generate() % 2 == 1)?-8.f:8.f;

        rackets[0].setSize(sf::Vector2f(racketW, racketH));
        rackets[1].setSize(sf::Vector2f(racketW, racketH));

        rackets[0].setOutlineThickness(-2.f);
        rackets[1].setOutlineThickness(-2.f);

        rackets[0].setOutlineColor(sf::Color(128, 128, 128));
        rackets[1].setOutlineColor(sf::Color(128, 128, 128));

        rackets[0].setOrigin(sf::Vector2f(racketW, racketH/2));
        rackets[1].setOrigin(sf::Vector2f(0.f, racketH/2));

        rackets[0].setPosition(racketW, windowY/2);
        rackets[1].setPosition(windowX - racketW, windowY/2);

        aBall.setRadius(ballR);
        aBall.setOutlineColor(sf::Color(128, 128, 128));
        aBall.setOutlineThickness(-3.f);

        aBall.setOrigin(ballR, ballR);
        aBall.setPosition(windowX/2, windowY/2);

        setSounds();
    }

    void checkMovement() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            if (rackets[0].getPosition().y >= rackets[0].getSize().y/2) rackets[0].move(-racketSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            if (rackets[0].getPosition().y <= windowY - rackets[0].getSize().y/2) rackets[0].move(racketSpeed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
            if (rackets[1].getPosition().y >= rackets[0].getSize().y/2) rackets[1].move(-racketSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
            if (rackets[1].getPosition().y <= windowY - rackets[0].getSize().y/2) rackets[1].move(racketSpeed);
        }

        aBall.move(ballSpeed);
    }

    void checkCollision() {
        const float amount = 130.f;
        float collisionPoint;

        double countDist0LowerSq = pow(aBall.getPosition().x - (rackets[0].getPosition().x + rackets[0].getSize().x/2), 2)\
        + pow(aBall.getPosition().y - (rackets[0].getPosition().y + rackets[0].getSize().y/2), 2);

        double countDist0UpperSq = pow(aBall.getPosition().x - (rackets[0].getPosition().x + rackets[0].getSize().x/2), 2)\
        + pow(aBall.getPosition().y - (rackets[0].getPosition().y - rackets[0].getSize().y/2), 2);

        double countDist1LowerSq = pow(aBall.getPosition().x - (rackets[1].getPosition().x - rackets[1].getSize().x/2), 2)\
        + pow(aBall.getPosition().y - (rackets[1].getPosition().y + rackets[1].getSize().y/2), 2);

        double countDist1UpperSq = pow(aBall.getPosition().x - (rackets[1].getPosition().x - rackets[1].getSize().x/2), 2)\
        + pow(aBall.getPosition().y - (rackets[1].getPosition().y - rackets[1].getSize().y/2), 2);

        float randomNum = (generate() % 100 + 50) / 100.f;

        if (aBall.getPosition().y <= aBall.getRadius()) {
            std::cerr << "Bounced T" << std::endl;
            toPlay.setBuffer(sounds[0]);
            toPlay.play();
            ballSpeed.y *= -1;
        }
        else if (aBall.getPosition().y >= windowY - aBall.getRadius()) {
            std::cerr << "Bounced B" << std::endl;
            toPlay.setBuffer(sounds[0]);
            toPlay.play();
            ballSpeed.y *= -1;
        }

        if ((rackets[0].getPosition().x + aBall.getRadius() >= aBall.getPosition().x &&\
        (aBall.getPosition().y <= rackets[0].getPosition().y + rackets[0].getSize().y/2 &&\
        aBall.getPosition().y >= rackets[0].getPosition().y - rackets[0].getSize().y/2)) ||
                (countDist0LowerSq <= pow(aBall.getRadius(), 2) && aBall.getPosition().y >= rackets[0].getPosition().y\
                +rackets[0].getSize().y/2 ) ||
                (countDist0UpperSq <= pow(aBall.getRadius(), 2) && aBall.getPosition().y <= rackets[0].getPosition().y\
                - rackets[0].getSize().y/2 )) {
            std::cerr << "Bounced LP" << std::endl;
            std::cerr << aBall.getPosition().x << " " << aBall.getPosition().y << std::endl;
            toPlay.setBuffer(sounds[1]);
            toPlay.play();
            ballSpeed.x *= -1;
            collisionPoint = aBall.getPosition().y - rackets[0].getPosition().y;
            if (collisionPoint != 0) {
                std::cerr << collisionPoint << std::endl;
                ballSpeed.y = collisionPoint/8;
            }
            ballSpeed.y *= randomNum;
            std::cerr << ballSpeed.x << " " << ballSpeed.y << std::endl;
        }
        else if ((rackets[1].getPosition().x - aBall.getRadius() <= aBall.getPosition().x &&\
        (aBall.getPosition().y <= rackets[1].getPosition().y + rackets[1].getSize().y/2 &&\
        aBall.getPosition().y >= rackets[1].getPosition().y - rackets[1].getSize().y/2)) ||
                 (countDist1LowerSq <= pow(aBall.getRadius(), 2) && aBall.getPosition().y >= rackets[1].getPosition().y\
                +rackets[1].getSize().y/2 ) ||
                 (countDist1UpperSq <= pow(aBall.getRadius(), 2) && aBall.getPosition().y <= rackets[1].getPosition().y\
                - rackets[1].getSize().y/2 )) {
            std::cerr << "Bounced RP" << std::endl;
            toPlay.setBuffer(sounds[1]);
            toPlay.play();
            ballSpeed.x *= -1;
            collisionPoint = aBall.getPosition().y - rackets[1].getPosition().y;
            if (collisionPoint != 0) {
                std::cerr << collisionPoint << std::endl;
                ballSpeed.y = collisionPoint/8;
            }
            ballSpeed.y *= randomNum;
            std::cerr << ballSpeed.x << " " << ballSpeed.y << std::endl;
        }

        if (aBall.getPosition().x - aBall.getRadius() <= 0) {
            std::cerr << "Out of Bounds L" << std::endl;
            toPlay.setBuffer(sounds[2]);
            toPlay.play();
            result[1] += 1;
            aBall.setPosition(windowX/2, windowY/2);
            ballSpeed.x = (generate() % 2 == 1)?-8.f:8.f;
            ballSpeed.y = 0;
            rackets[0].setPosition(rackets[0].getSize().x, windowY/2);
            rackets[1].setPosition(windowX - rackets[0].getSize().x, windowY/2);
        }
        else if (aBall.getPosition().x + aBall.getRadius() >= windowX) {
            std::cerr << "Out of Bounds R" << std::endl;
            toPlay.setBuffer(sounds[2]);
            toPlay.play();
            result[0] += 1;
            aBall.setPosition(windowX/2, windowY/2);
            ballSpeed.x = (generate() % 2 == 1)?-8.f:8.f;
            ballSpeed.y = 0;
            rackets[0].setPosition(rackets[0].getSize().x, windowY/2);
            rackets[1].setPosition(windowX - rackets[0].getSize().x, windowY/2);
        }
    }

    sf::RectangleShape& getRacket(int num) {
        class BadIndex {};
        if (!(num < 0 || num > 2)) {
            return rackets[num];
        }
        throw BadIndex();
    }

    sf::CircleShape& getBall() {
        return aBall;
    }

    int* getResult() {
        return result;
    }
};

int main()
{
    const int winX = 800, winY = 500;
    const float ballR = 15.f;
    const float racketW = 10.f, racketH = 100.f;

    std::chrono::milliseconds frame(17);
    std::string result;

    Pong gameS(winX, winY);
    gameS.init(ballR, racketW, racketH);

    sf::SoundBuffer soundBuf, soundBuf2;
    sf::Sound sound, sound2;

    sf::Font basicFont;
    if (!basicFont.loadFromFile("impact.ttf")) std::cerr << "FAILED TO LOAD FONT" << std::endl;

    sf::Text resultText;
    resultText.setPosition(winX - 500.f, winY - 150.f);
    resultText.setFont(basicFont);
    resultText.setOutlineThickness(3.f);
    resultText.setCharacterSize(140);
    std::cerr << resultText.getCharacterSize() << " " << resultText.getLetterSpacing() << " " << resultText.getLineSpacing() << std::endl;
    resultText.setOutlineColor(sf::Color(128, 128, 128));

    soundBuf.loadFromFile("./ping_pong_8bit_plop.wav");
    soundBuf2.loadFromFile("./ping_pong_8bit_beeep.wav");

    sound.setBuffer(soundBuf);
    sound2.setBuffer(soundBuf2);

    sf::RenderWindow window(sf::VideoMode(winX, winY), "Pong or Not?");
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        result = std::to_string(*gameS.getResult()) + " : " + std::to_string(*(gameS.getResult()+1));
        resultText.setString(result);

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() >=  frame.count()) {
            gameS.checkMovement();
            gameS.checkCollision();
            start = std::chrono::steady_clock::now();
        }

        window.clear();
        window.draw(resultText);
        window.draw(gameS.getRacket(0));
        window.draw(gameS.getRacket(1));
        window.draw(gameS.getBall());
        window.display();
    }

    return 0;
}