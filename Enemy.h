#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Animation.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
struct Bullet {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float gravity;
    bool active;
    Animation animation;

    Bullet() : gravity(300.f), active(false) {}

    void init(sf::Vector2f pos, sf::Vector2f vel, const std::string& texturePath, const int frameW, const int frameH, int frames, const float frameTime, const int cropX, const int cropW, const float scale) {
        position = pos;
        velocity = vel;
        active = true;
        animation.load(texturePath, frameW, frameH, frames, frameTime, cropX, cropW);
        animation.getSprite().setScale(2.5f, 2.5f);
        animation.getSprite().setPosition(position);

    }

    bool CheckCollisions(PlayerCharacter* p) {
        sf::Vector2f pos = animation.getSprite().getPosition();
        float distance = std::sqrt(std::pow(pos.x - p->getPositionX(), 2.0f) + std::pow(pos.y - p->getPositionY(), 2.0f));
        float threshold = 70.0f;
        if (distance < threshold) {
            return true;
        }
        else
            return false;
    }
    void update(float dt) {
        if (!active) return;
        velocity.y += gravity * dt;
        position += velocity * dt;
        animation.getSprite().setPosition(position);

        animation.play();
        animation.update(dt);
    }

    void render(sf::RenderWindow& window) {
        if (active)
            window.draw(animation.getSprite());
    }
};
class EnemyCharacter {
public:

    EnemyCharacter(float startX, float startY);
    virtual ~EnemyCharacter() = default;
    int getHP()const { return enemyHP; }
    void decreaseLives(float dt) { enemyHP--; }
    void setPosition(float nx, float ny);
    int getSpriteWidth() const { return spriteWidth; }
    int getSpriteHeight() const { return spriteHeight; }
    float getVelocityX() const { return velocityX; }
    float getVelocityY() const { return velocityY; }
    void setScale(float sx, float sy);
    void setTexture(const std::string& path);
    virtual void update(float dt, Level& level);
    virtual void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    float getPositionX() const { return { x }; }
    float getPositionY() const { return { y }; }
    void applyGravity(float dt, const Level& level);
    sf::Vector2f calculateProjectileVelocity(sf::Vector2f origin, sf::Vector2f target, float speed);
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float velocityY;
    float x, y;
    float velocityX;
    float minX, maxX;
    bool isfiring;
    int direction;
    float spriteWidth, spriteHeight;
    Animation animWalkR, animWalkL;
    Animation* current;
    void updateDimensions();
    bool onGround = false;
    const float gravity = 0.5f;
    const float terminalVel = 12.f;
    int enemyHP;


};
