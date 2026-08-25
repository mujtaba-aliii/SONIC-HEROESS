#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    void run() const; 
    void levelSelection() const; 
protected:
    int selectedItemIndex = 0;
    bool isUpPressed = false;
    bool isDownPressed = false;
    sf::Clock inputClock;
    sf::Event event;
    sf::Text menu[5]; 
    std::string mainOptions[5] = { "NEW GAME", "SELECT LEVEL", "OPTIONS", "CONTINUE", "LEADERBOARD" };
    std::string levelOptions[5] = { "Level 1", "Level 2", "Level 3", "Level 4", "Exit" };
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Font font;

    bool MenuActive = true;
};

