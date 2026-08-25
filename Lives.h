#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include "PlayerCharacter.h"
class Lives {
public:
	Lives(float startX, float startY)
		: x(startX), y(startY)
	{
		if (!font.loadFromFile("Data/arial.ttf")) {
			std::cerr << "Failed to load font\n";
		}
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setPosition(x, y);
	}
	void setLives(int lives) {
		this->lives = lives;
		text.setString("Lives: " + std::to_string(lives));
	}
	void render(sf::RenderWindow& window) {
		window.draw(text);
	}
protected:
	float x, y;
	int lives = 3; 
	sf::Font font;
	sf::Text text;
	sf::Texture texture;
};