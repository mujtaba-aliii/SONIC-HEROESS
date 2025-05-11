#pragma once

#include <SFML/Graphics.hpp>
#include "Level.h"

class PlayerCharacter {
public:
    PlayerCharacter(float gravity, float terminalVel,
        float maxSpeed, float acceleration,
        float deceleration);
    virtual ~PlayerCharacter() = default;

    virtual void update(float dt, Level& level,
        int windowWidth, int windowHeight);
    virtual void render(sf::RenderWindow& window);
    virtual void handleInput(const sf::Event& ev);
    void toggleDebug();

    void applyMovement(float dt, Level& level);
    void applyGravity(float dt, Level& level);

    virtual void followTarget(float targetX, float targetY,
        float activeVelX, float dt,
        Level& level);

    sf::Vector2f getPosition()    const { return { x, y }; }
    float        getPositionX()   const { return x; }
    float        getPositionY()   const { return y; }
    float        getVelocityX()   const { return velocityX; }
    float        getVelocityY()   const { return velocityY; }
	float getMaxSpeed() const { return maxSpeed; }
    bool         isOnGround()     const { return onGround; }
    bool         isMoving()       const { return movingLeft || movingRight; }

    int getSpriteWidth() const { return spriteWidth; }
    int getSpriteHeight()const { return spriteHeight; }

    void setGravity(float g) { gravity = g; }
    void setTerminalVel(float tv) { terminalVel = tv; }
    void setMaxSpeed(float ms) { maxSpeed = ms; }
    void setAcceleration(float a) { acceleration = a; }
    void setDeceleration(float d) { deceleration = d; }
	void setVelocityY(float v) { velocityY = v; }
	void setOnGround(bool g) { onGround = g; }

    void setPosition(float nx, float ny) { x = nx; y = ny; sprite.setPosition(x, y); }
    void setScale(float sx, float sy) { sprite.setScale(sx, sy); updateDimensions(); }

	void setTexture(const std::string& filename);
    void clampPosition(int windowWidth, int windowHeight, int levelWidth);
protected:
    sf::Sprite  sprite;
    sf::Texture texture;

    float x, y;
    float velocityX, velocityY;
    bool onGround;
    bool movingLeft, movingRight, debugMode;

    float gravity;
    float terminalVel;
    float maxSpeed;
    float jumpStrength = -20.f;
    float acceleration;
    float deceleration;

    float spriteWidth, spriteHeight;

    void updateDimensions();
    void checkMovementKeys();
};
