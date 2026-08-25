#include "Menu.h"
#include "Options.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "Game.h"
using namespace std;

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

    sf::Text menu[5];
    std::string mainOptions[5] = { "NEW GAME", "SELECT LEVEL", "OPTIONS", "CONTINUE", "LEADERBOARD" };
    for (int i = 0; i < 5; ++i) {
        menu[i].setFont(font);
        menu[i].setString(mainOptions[i]);
        menu[i].setCharacterSize(40);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setPosition(180, 330 + (i - 3) * 70); 
    }

    int selectedItemIndex = 0;
    bool isUpPressed = false;
    bool isDownPressed = false;
    sf::Clock inputClock;
    sf::Music music;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (inputClock.getElapsedTime().asMilliseconds() > 200) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isUpPressed) {
                selectedItemIndex = (selectedItemIndex - 1 + 5) % 5;
                isUpPressed = true;
                inputClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isDownPressed) {
                selectedItemIndex = (selectedItemIndex + 1) % 5;
                isDownPressed = true;
                inputClock.restart();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (selectedItemIndex == 0) { 
                    Game game(1600, 900, 1);
                    game.run();
                }
                else if (selectedItemIndex == 1) { 
                    levelSelection();
                }
                else if (selectedItemIndex == 2) { 
                    Button optionsButton(100, 500, "Menu");
                    optionsButton.run(window, &music);
                }
                else if (selectedItemIndex == 3) { 

                }
                else if (selectedItemIndex == 4) { 

                }
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            isUpPressed = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            isDownPressed = false;
        }

        for (int i = 0; i < 5; ++i) {
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

        for (int i = 0; i < 5; ++i) {
            window.draw(menu[i]);
        }

        window.display();
    }
}

void Menu::levelSelection() const {
    sf::RenderWindow window(sf::VideoMode(1080, 600), "Select Level");

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
    std::string levelOptions[4] = { "Level 1", "Level 2", "Level 3", "Level 4"};
    for (int i = 0; i < 4; ++i) {
        menu[i].setFont(font);
        menu[i].setString(levelOptions[i]);
        menu[i].setCharacterSize(40);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setPosition(180, 330 + (i - 3) * 70); 
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
                selectedItemIndex = (selectedItemIndex - 1 + 5) % 5;
                isUpPressed = true;
                inputClock.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isDownPressed) {
                selectedItemIndex = (selectedItemIndex + 1) % 5;
                isDownPressed = true;
                inputClock.restart();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (selectedItemIndex == 0) { // Level 1
                    Game game(1600, 900,1);
                    game.run();
                }
                else if (selectedItemIndex == 1) { // Level 2
                    Game game(1600, 900, 2);
                    game.run();
                }
                else if (selectedItemIndex == 2) { // Level 3
                    Game game(1600, 900, 3);
                    game.run();
                }
                else if (selectedItemIndex == 3) { // Level 4
                    Game game(1600, 900, 4);
                    game.run();
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

