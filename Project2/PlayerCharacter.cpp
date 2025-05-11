#include "PlayerCharacter.h"
#include <algorithm>
#include <iostream>

PlayerCharacter::PlayerCharacter(float gravity_, float terminalVel_,
    float maxSpeed_, float acceleration_,
    float deceleration_)
    : x(0), y(0),
    velocityX(0), velocityY(0),
    onGround(false),
    movingLeft(false), movingRight(false), debugMode(false),
    gravity(gravity_),
    terminalVel(terminalVel_),
    maxSpeed(maxSpeed_),
    jumpStrength(-20.f),
    acceleration(acceleration_),
    deceleration(deceleration_),
    spriteWidth(64), spriteHeight(64)
{
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

void PlayerCharacter::setTexture(const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }
    sprite.setTexture(texture);
    updateDimensions();
}

void PlayerCharacter::render(sf::RenderWindow& window) {
    if (texture.getSize().x > 0)
        window.draw(sprite);
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



void PlayerCharacter::toggleDebug() {
    debugMode = !debugMode;
    std::cout << "Debug: " << (debugMode ? "ON" : "OFF") << std::endl;
}

void PlayerCharacter::followTarget(float targetX, float targetY, float activeVelX, float dt, Level& level) {
    // 1) Adopt active player’s horizontal speed
    velocityX = activeVelX;

    // 2) Move horizontally and check for collisions
    float newX = x + velocityX * dt * 60.f;
    bool collL = level.collidesAt(newX + 8, y + spriteHeight - 2);
    bool collR = level.collidesAt(newX + spriteWidth - 8, y + spriteHeight - 2);
    if (!collL && !collR) {
        x = newX;
    }
    else {
        velocityX = 0;
    }

    // 3) Adjust vertical position if follower is off the platform
    if (onGround && (y > targetY + 20.f)) {
        velocityY = jumpStrength;
        onGround = false;
    }

    // 4) Apply gravity and check for platform collisions
    applyGravity(dt, level);

    // Check if the follower is standing on a platform
    for (int i = 0; i < level.getPlatformsCount(); ++i) {
        float platformX = level.getPlatformsX(i);
        float platformY = level.getPlatformsY(i);
        float platformWidth = level.getCellSize();
        float platformHeight = level.getCellSize();

        bool isOnPlatform = (x + spriteWidth > platformX && x < platformX + platformWidth) &&
            (y + spriteHeight > platformY && y + spriteHeight < platformY + platformHeight);

        if (isOnPlatform) {
            y = platformY - spriteHeight; // Align with the top of the platform
            velocityY = 0;
            onGround = true;
            break;
        }
    }

    // 5) Final position commit
    sprite.setPosition(x, y);
}void PlayerCharacter::updateDimensions() {
    spriteWidth = texture.getSize().x * sprite.getScale().x;
    spriteHeight = texture.getSize().y * sprite.getScale().y;
}

void PlayerCharacter::checkMovementKeys() {
    movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D);
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

    // Check for ground collisions
    bool hitL = level.collidesAt(x + 8, newY + spriteHeight);
    bool hitR = level.collidesAt(x + spriteWidth - 8, newY + spriteHeight);

    if (hitL || hitR) {
        onGround = true;
        int cell = level.getCellSize();
        int blockY = (int(newY + spriteHeight) / cell) * cell;
        y = blockY - spriteHeight;
        velocityY = 0;
    }
    else {
        // Check for platform collisions
        onGround = false;
        for (int i = 0; i < level.getPlatformsCount(); ++i) {
            float platformX = level.getPlatformsX(i);
            float platformY = level.getPlatformsY(i);
            float platformWidth = level.getCellSize();
            float platformHeight = level.getCellSize();

            bool isOnPlatform = (x + spriteWidth > platformX && x < platformX + platformWidth) &&
                (newY + spriteHeight > platformY && newY + spriteHeight < platformY + platformHeight);

            if (isOnPlatform) {
                y = platformY - spriteHeight; // Align with the top of the platform
                velocityY = 0;
                onGround = true;
                break;
            }
        }

        if (!onGround) {
            y = newY;
        }
    }
}
void PlayerCharacter::clampPosition(int windowWidth,
    int windowHeight,
    int levelWidth)
{
    if (x < 0) { x = 0; velocityX = 0; }
    float maxX = levelWidth - spriteWidth - (spriteWidth / 8.f);
    if (x > maxX) { x = maxX; velocityX = 0; }
    if (y < 0) { y = 0; velocityY = 0; }
    sprite.setPosition(x, y);
}
