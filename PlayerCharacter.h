#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"

class PlayerCharacter {
public:
    PlayerCharacter(float startX, float startY);
    ~PlayerCharacter() = default;

    void setTexture(const std::string& filename);
    virtual void update(float dt, Level& level);
    virtual void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return { x,y }; }
    float getPositionX() const { return { x }; }
    float getPositionY() const { return { y }; }
    void setPosition(float nx, float ny) { x = nx; y = ny; sprite.setPosition(x, y); }
    int getSpriteWidth() const { return spriteWidth; }
    int getSpriteHeight() const { return spriteHeight; }
    float getVelocityX() const { return velocityX; }
    bool isOnGround()   const { return onGround; }
    void setScale(float sx, float sy) { sprite.setScale(sx, sy); updateDimensions(); }

    void applyMovement(float dt, Level& level);
    void applyGravity(float dt, Level& level);
    void toggleDebug();
    void handleInput(const sf::Event& event);

protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float x, y, velocityX, velocityY;
    bool onGround;
    const float gravity = 0.8f, terminalVel = 20.f, maxSpeed = 15.f,
        jumpStrength = -20.f, acceleration = 1.0f, deceleration = 0.8f;
    float spriteWidth, spriteHeight;
    bool movingLeft, movingRight, debugMode;

    void updateDimensions();
    void checkMovementKeys();
};

