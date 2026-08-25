#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.h"
#include "Animation.h"
#include "Level.h"

EnemyCharacter::EnemyCharacter(float startX, float startY)
    : x(startX), y(startY), velocityX(50.f), direction(1), velocityY(50.f),
    animWalkR(), animWalkL(), current(&animWalkR) {
}



sf::Vector2f EnemyCharacter::calculateProjectileVelocity(sf::Vector2f origin, sf::Vector2f target, float speed) {
    sf::Vector2f toTarget = target - origin;
    float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    if (distance == 0) return { 0.f, 0.f };
    sf::Vector2f direction = toTarget / distance;
    return direction * speed;
}
void EnemyCharacter::setPosition(float nx, float ny) {
    x = nx;
    y = ny;
    sprite.setPosition(x, y);
}

void EnemyCharacter::update(float dt, Level& level) {


    current->play();
    current->update(dt);
    current->getSprite().setPosition(x, y);
}

void EnemyCharacter::applyGravity(float dt, const Level& level) {
    const float gravity = 1000.0f;
    velocityY += gravity * dt;

    float nextY = y + velocityY * dt;

    float footY = nextY + spriteHeight;
    bool onGround = level.collidesAt(x + 8, footY, 0) || level.collidesAt(x + spriteWidth - 8, footY, 0);

    if (!onGround) {
        y = nextY;
    }
    else {
        y = std::floor((footY) / level.getCellSize()) * level.getCellSize() - spriteHeight;
        velocityY = 0;
    }

    sprite.setPosition(x, y);
}


void EnemyCharacter::setTexture(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    else {
        sprite.setTexture(texture);
        updateDimensions();
    }
}

void EnemyCharacter::render(sf::RenderWindow& window) {
    window.draw(current->getSprite());
}
void EnemyCharacter::updateDimensions() {
    spriteWidth = sprite.getGlobalBounds().width;
    spriteHeight = sprite.getGlobalBounds().height;
}


void EnemyCharacter::setScale(float sx, float sy) {
    animWalkR.getSprite().setScale(sx, sy);
    animWalkL.getSprite().setScale(sx, sy);
}


