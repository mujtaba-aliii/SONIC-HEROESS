#include "Menu.h"
#include "Options.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include"Game.h"
using namespace std;

void start_level2(sf::RenderWindow& window) {}
void start_level3(sf::RenderWindow& window) {}

void Menu::run() const {
    srand(static_cast<unsigned>(time(0)));
    bool MenuActive = true;

    sf::RenderWindow window(sf::VideoMode(1080, 600), "Main Menu");

    sf::Font font;
    if (!font.loadFromFile("Data/font.ttf")) {
        return;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Data/background.jpg")) {
        cout << "failed!!!" << endl;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Text menu[4];
    std::string options[4] = { "NEW GAME", "OPTIONS", "CONTINUE", "LEADERBOARD" };
    for (int i = 0; i < 4; ++i) {
        menu[i].setFont(font);
        menu[i].setString(options[i]);
        menu[i].setCharacterSize(40);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setPosition(180, 330 + (i - 3) * 70); // Adjust as needed
    }

    int selectedItemIndex = 0;
    bool isUpPressed = false;
    bool isDownPressed = false;
    sf::Clock inputClock;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (inputClock.getElapsedTime().asMilliseconds() > 200) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isUpPressed) {
                selectedItemIndex = (selectedItemIndex - 1 + 4) % 4;
                isUpPressed = true;
                inputClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isDownPressed) {
                selectedItemIndex = (selectedItemIndex + 1) % 4;
                isDownPressed = true;
                inputClock.restart();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (selectedItemIndex == 0) {
                    Game game(1600, 900);
                    game.run();
                }
                else if (selectedItemIndex == 1) {
                    Button optionsButton(100, 500, "Menu");
                    optionsButton.run(window);
                }
                else if (selectedItemIndex == 2) {
                    start_level3(window);
                }
                else if (selectedItemIndex == 3) {
                    window.close();
                }
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            isUpPressed = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            isDownPressed = false;
        }

        for (int i = 0; i < 4; ++i) {
            if (i == selectedItemIndex) {
                menu[i].setFillColor(sf::Color::Red);
                menu[i].setCharacterSize(50);
            }
            else {
                menu[i].setFillColor(sf::Color::White);
                menu[i].setCharacterSize(40);
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        for (int i = 0; i < 4; ++i) {
            window.draw(menu[i]);
        }

        window.display();
    }
}

