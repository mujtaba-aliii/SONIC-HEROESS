// PlayerCharacter.cpp
#include "PlayerCharacter.h"
#include <algorithm>
#include <iostream>

PlayerCharacter::PlayerCharacter(float startX, float startY)
    : x(startX), y(startY),
    velocityX(0), velocityY(0),
    onGround(false),
    spriteWidth(64), spriteHeight(64),
    movingLeft(false), movingRight(false),
    debugMode(false)
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
}

void PlayerCharacter::handleInput(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if ((ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::W) && onGround) {
            velocityY = jumpStrength;
            onGround = false;
        }
    }
    else if (ev.type == sf::Event::KeyReleased) {
        if (ev.key.code == sf::Keyboard::Left || ev.key.code == sf::Keyboard::A) {
            if (movingLeft) {
                movingLeft = false;
                if (velocityX < 0) velocityX = 0;
            }
        }
        else if (ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::D) {
            if (movingRight) {
                movingRight = false;
                if (velocityX > 0) velocityX = 0;
            }
        }
    }
    checkMovementKeys();
}

void PlayerCharacter::checkMovementKeys() {
    movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}

void PlayerCharacter::applyMovement(float dt, Level& level) {
    if (movingLeft) {
        velocityX = std::max(velocityX - acceleration * dt * 60.f, -maxSpeed);
    }
    else if (movingRight) {
        velocityX = std::min(velocityX + acceleration * dt * 60.f, maxSpeed);
    }
    else {
        if (velocityX > 0) velocityX = std::max(0.f, velocityX - deceleration * dt * 60.f * 3);
        else              velocityX = std::min(0.f, velocityX + deceleration * dt * 60.f * 3);
    }

    float newX = x + velocityX * dt * 60.f;
    bool collL = level.collidesAt(newX + 8, y + spriteHeight - 2),
        collR = level.collidesAt(newX + spriteWidth - 8, y + spriteHeight - 2);

    if (!collL && !collR) x = newX;
    else                  velocityX = 0;
}

void PlayerCharacter::applyGravity(float dt, Level& level) {
    velocityY = std::min(velocityY + gravity * dt * 60.f, terminalVel);
    float newY = y + velocityY * dt * 60.f;
    bool hitL = level.collidesAt(x + 8, newY + spriteHeight),
        hitR = level.collidesAt(x + spriteWidth - 8, newY + spriteHeight);

    if (hitL || hitR) {
        onGround = true;
        int cell = level.getCellSize();
        int blockY = (int(newY + spriteHeight) / cell) * cell;
        y = blockY - spriteHeight;
        velocityY = 0;
    }
    else {
        onGround = false;
        y = newY;
    }
}

void PlayerCharacter::update(float dt, Level& level, int windowWidth, int windowHeight) {
    checkMovementKeys();
    applyMovement(dt, level);
    applyGravity(dt, level);
    sprite.setPosition(x, y);

    // Pass the level width to clampPosition
    clampPosition(windowWidth, windowHeight, level.getWidth() * level.getCellSize());

    sprite.setPosition(x, y);
}



void PlayerCharacter::render(sf::RenderWindow& window) {
    // 1) draw the character
    if (texture.getSize().x > 0)
        window.draw(sprite);
}


void PlayerCharacter::toggleDebug() {
    debugMode = !debugMode;
    std::cout << "Debug: " << (debugMode ? "ON" : "OFF") << std::endl;
}

void PlayerCharacter::followTarget(float targetX, float targetY, float activeVelX, float dt, Level& level) {
    // 1) adopt active’s horizontal speed exactly
    velocityX = activeVelX;

    // 2) x move & collision
    float newX = x + velocityX * dt * 60.f;
    bool collL = level.collidesAt(newX + 8, y + spriteHeight - 2),
        collR = level.collidesAt(newX + spriteWidth - 8, y + spriteHeight - 2);
    if (!collL && !collR) x = newX;
    else velocityX = 0;

    // 3) adjust vertical if follower is off the platform
    if (onGround && (y > targetY + 20.f)) {
        velocityY = jumpStrength;
        onGround = false;
    }
    applyGravity(dt, level);

    // 4) final position commit
    sprite.setPosition(x, y);
}
