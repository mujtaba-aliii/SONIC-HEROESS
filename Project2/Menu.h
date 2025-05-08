#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    void run() const; // Method to show the menu
protected:
    int selectedItemIndex = 0;
    bool isUpPressed = false;
    bool isDownPressed = false;
    sf::Clock inputClock;
    sf::Event event;
    sf::Text menu[4];
    std::string options[4] = { "Start level1", "Start level2", "Start level3", "Exit" };
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Font font;

    bool MenuActive = true;


};
