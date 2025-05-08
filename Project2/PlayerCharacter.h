#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"

class PlayerCharacter {
public:
    PlayerCharacter(float startX, float startY);
    virtual ~PlayerCharacter() = default;
    void setMaxSpeed(float ms) { maxSpeed = ms; }

    void setTexture(const std::string& filename);
    virtual void update(float dt, Level& level, int windowWidth, int windowHeight);
    virtual void render(sf::RenderWindow& window);
    virtual void handleInput(const sf::Event& ev);
    void toggleDebug();

    sf::Vector2f getPosition()    const { return { x, y }; }
    float getPositionX() const { return { x }; }
    float getPositionY() const { return { y }; }
    float getVelocityX() const { return velocityX; }
    bool         isOnGround()     const { return onGround; }
    int          getSpriteWidth() const { return spriteWidth; }
    int          getSpriteHeight()const { return spriteHeight; }

    void setPosition(float nx, float ny) { x = nx; y = ny; sprite.setPosition(x, y); }
    void setScale(float sx, float sy) { sprite.setScale(sx, sy); updateDimensions(); }

    virtual void followTarget(float targetX, float targetY, float activeVelX, float dt, Level& level);

    void applyMovement(float dt, Level& level);
    void applyGravity(float dt, Level& level);

    bool isMoving() const { return movingLeft || movingRight; }
    void clampPosition(int windowWidth, int windowHeight, int levelWidth) {
        // Left bound
        if (x < 0) {
            x = 0;
            velocityX = 0; // Stop movement to the left
        }

        // Right bound (level width minus sprite width and an additional offset)
        float maxX = levelWidth - spriteWidth - (spriteWidth / 8); // Dynamically reduce the right bound
        if (x > maxX) {
            x = maxX;
            velocityX = 0; // Stop movement to the right
        }

        // Top bound
        if (y < 0) {
            y = 0;
            velocityY = 0; // Stop upward movement
        }

        // Bottom bound (handled by gravity and collision detection)
        sprite.setPosition(x, y);
    }



protected:
    sf::Sprite  sprite;
    sf::Texture texture;
    float x, y, velocityX, velocityY;
    bool onGround;

    // MADE MUTABLE:
    float gravity = 0.8f;
    float terminalVel = 20.f;
    float maxSpeed = 15.f;   // <-- now adjustable
    float jumpStrength = -20.f;
    float acceleration = 1.0f;
    float deceleration = 0.8f;

    float spriteWidth, spriteHeight;
    bool movingLeft, movingRight, debugMode;

    void updateDimensions();
    void checkMovementKeys();







};
