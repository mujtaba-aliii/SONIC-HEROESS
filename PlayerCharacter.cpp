#include "PlayerCharacter.h"
#include <iostream>
#include <algorithm>

PlayerCharacter::PlayerCharacter(float startX, float startY)
    : x(startX), y(startY), velocityX(0), velocityY(0), onGround(false),
    spriteWidth(64), spriteHeight(64), movingLeft(false), movingRight(false), debugMode(false)
{
}

void PlayerCharacter::setTexture(const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }
    sprite.setTexture(texture);
    updateDimensions();
}

void PlayerCharacter::updateDimensions() {
    spriteWidth = texture.getSize().x * sprite.getScale().x;
    spriteHeight = texture.getSize().y * sprite.getScale().y;
    if (debugMode) std::cout << "Sprite dimensions: " << spriteWidth << "x" << spriteHeight << std::endl;
}

void PlayerCharacter::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if ((event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) && onGround) {
            velocityY = jumpStrength;
            onGround = false;
            if (debugMode) std::cout << "Jump initiated!" << std::endl;
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
            if (movingLeft) {
                movingLeft = false;
                if (velocityX < 0) velocityX = 0;
                if (debugMode) std::cout << "Left key released" << std::endl;
            }
        }
        else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {
            if (movingRight) {
                movingRight = false;
                if (velocityX > 0) velocityX = 0;
                if (debugMode) std::cout << "Right key released" << std::endl;
            }
        }
    }
    checkMovementKeys();
}

void PlayerCharacter::checkMovementKeys() {
    bool wasL = movingLeft, wasR = movingRight;
    movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    if (debugMode && (wasL != movingLeft || wasR != movingRight))
        std::cout << "Movement keys - Left:" << movingLeft << " Right:" << movingRight << std::endl;
}

void PlayerCharacter::applyMovement(float dt, Level& level) {
    if (movingLeft) {
        velocityX = std::max(velocityX - acceleration * dt * 60.f, -maxSpeed);
        if (debugMode) std::cout << "Moving left " << velocityX << std::endl;
    }
    else if (movingRight) {
        velocityX = std::min(velocityX + acceleration * dt * 60.f, maxSpeed);
        if (debugMode) std::cout << "Moving right " << velocityX << std::endl;
    }
    else {
        if (velocityX > 0) velocityX = std::max(0.f, velocityX - deceleration * dt * 60.f * 3);
        else if (velocityX < 0) velocityX = std::min(0.f, velocityX + deceleration * dt * 60.f * 3);
    }

    float newX = x + velocityX * dt * 60.f;
    bool collL = level.collidesAt(newX + 8, y + spriteHeight - 2),
        collR = level.collidesAt(newX + spriteWidth - 8, y + spriteHeight - 2);
    if (!collL && !collR) {
        x = newX;
    }
    else {
        velocityX = 0;
        if (debugMode) std::cout << "Movement blocked" << std::endl;
    }
}

void PlayerCharacter::applyGravity(float dt, Level& level) {
    velocityY = std::min(velocityY + gravity * dt * 60.f, terminalVel);
    float newY = y + velocityY * dt * 60.f;
    bool hitL = level.collidesAt(x + 8, newY + spriteHeight),
        hitR = level.collidesAt(x + spriteWidth - 8, newY + spriteHeight);
    if (hitL || hitR) {
        onGround = true;
        int cellSize = level.getCellSize();
        int blockY = (int(newY + spriteHeight) / cellSize) * cellSize;
        y = blockY - spriteHeight;
        velocityY = 0;
        if (debugMode) std::cout << "On ground at y=" << y << std::endl;
    }
    else {
        onGround = false;
        y = newY;
        if (debugMode && velocityY != 0) std::cout << "In air velY=" << velocityY << std::endl;
    }
}

void PlayerCharacter::update(float dt, Level& level) {
    checkMovementKeys();
    applyMovement(dt, level);
    applyGravity(dt, level);
    sprite.setPosition(x, y);
}

void PlayerCharacter::render(sf::RenderWindow& window) {
    if (texture.getSize().x > 0) window.draw(sprite);
    if (debugMode) {
        sf::RectangleShape bl(sf::Vector2f(4, 4));
        bl.setPosition(x + 8, y + spriteHeight - 2);
        bl.setFillColor(sf::Color::Red);
        window.draw(bl);
        sf::RectangleShape br(sf::Vector2f(4, 4));
        br.setPosition(x + spriteWidth - 8, y + spriteHeight - 2);
        br.setFillColor(sf::Color::Red);
        window.draw(br);
    }
}

void PlayerCharacter::toggleDebug() {
    debugMode = !debugMode;
    std::cout << "Debug mode: " << (debugMode ? "ON" : "OFF") << std::endl;
}
