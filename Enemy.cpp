#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.h"
#include "Animation.h"
#include "Level.h"

EnemyCharacter::EnemyCharacter(float startX, float startY)
    : x(startX), y(startY), velocityX(50.f), direction(1),
    animWalkR(), animWalkL(), current(&animWalkR) {
}

void EnemyCharacter::setPosition(float nx, float ny) {
	x = nx;
	y = ny;
	sprite.setPosition(x, y);
}
void EnemyCharacter::update(float dt, Level& level) {
    x += velocityX * dt;
    current->update(dt);
    current->getSprite().setPosition(x, y);
}

void EnemyCharacter::setTexture(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    else {
        sprite.setTexture(texture);
        updateDimensions();  // Ensure dimensions are updated after setting the texture
    }
}

void EnemyCharacter::render(sf::RenderWindow& window) {
    window.draw(current->getSprite());
}
void EnemyCharacter::updateDimensions() {
	spriteWidth = sprite.getTexture()->getSize().x;
	spriteHeight = sprite.getTexture()->getSize().y;
}


void EnemyCharacter::setScale(float sx, float sy) {
    animWalkR.getSprite().setScale(sx, sy);
    animWalkL.getSprite().setScale(sx, sy);
}


