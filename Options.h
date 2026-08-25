#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Button {
public:
    void run(sf::RenderWindow& window, sf::Music* music);
    Button(float x, float y, const std::string& text) {
        button.setSize(sf::Vector2f(200.f, 200.f));
        button.setPosition(x + 50, y - 50);
        button.setFillColor(sf::Color::Blue);

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(100);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(x + 25.f, y + 15.f);

    }

    void setFont(const sf::Font& font) {
        this->font = font;
        buttonText.setFont(font);
    }

    bool isClicked(const sf::Vector2i& mousePos) const {
        return button.getGlobalBounds().contains(sf::Vector2f(mousePos));
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(button);
        window.draw(buttonText);
    }

private:
    sf::RectangleShape button;
    sf::Text buttonText;
    sf::Font font;
};


