#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include "Animation.h"
using namespace std;

class collectibles {
public:
	collectibles(float startX, float startY)
		: x(startX), y(startY), velocityX(1.5f), direction(1)
	{
		const int frameW = 16;       // width of one frame
		const int frameH = 16;       // height of one frame
		const float frameTime = 0.1f; // slightly slower, since the image is small
		const int cropX = 0;
		const int cropW = frameW;    // cropping the whole frame (not cutting anything)
		const float scale = 4.0f;    // enlarge 4x for better visibility

		spriteWidth = frameW;
		spriteHeight = 0;

        animWalkR.load("Data/ring.png", frameW, frameH, 4, frameTime, cropX, cropW);

		animWalkR.getSprite().setScale(scale, scale);
	

		current = &animWalkR;
		current->getSprite().setPosition(x, y);
	}
	int getSpriteWidth() const { return spriteWidth; }
	int getSpriteHeight() const { return spriteHeight; }
	void render(sf::RenderWindow& window) {
		window.draw(current->getSprite());
	}
	void setScale(float sx, float sy) {
		animWalkR.getSprite().setScale(sx+1, sy+1);
		
	}
	void setTexture() {
		if (!texture.loadFromFile("Data/ring.png")) {
			std::cout << "Failed to load texture: Data/enemy.png" << std::endl;
		}
		current->getSprite().setTexture(texture);

		
	}
	void update(float dt,PlayerCharacter* p) {
		if (current)
			current->update(dt);  // Keep only if collectible is animated (like spinning rings)
		 RingCollisions( p);
	}

	void setPosition(float nx, float ny) {
		x = nx;
		y = ny;

		if (current)
			current->getSprite().setPosition(x, y);  // Keep only if using animation sprites
		else
			sprite.setPosition(x, y);  // Backup if using plain sf::Sprite
	}

	void updateDimensions() {
		if (sprite.getTexture()) {
			spriteWidth = sprite.getTexture()->getSize().x;
			spriteHeight = sprite.getTexture()->getSize().y;
		}
	}
	void RingCollisions(PlayerCharacter* p) {

		float distance = std::sqrt(std::pow(x - p->getPositionX(), 2.0f) + std::pow(y - p->getPositionY(), 2.0f));

		// Print positions to debug
		std::cout << "Ring Position: " << x << ", " << y << std::endl;
		std::cout << "Player Position: " << p->getPositionX() << ", " << p->getPositionY() << std::endl;

		// Test threshold to detect collisions
		float threshold = 50.0f; // Adjust this value
		if (distance < threshold) {
			setPosition(-100, -100); // Move the ring off-screen
			std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>COLLISION<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		}
	}
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	float x, y;
	float velocityX;
	int direction;
	int spriteWidth, spriteHeight;
	Animation animWalkR;
	Animation* current=nullptr;
};
