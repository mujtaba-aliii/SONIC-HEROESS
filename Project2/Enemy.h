#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Animation.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
class EnemyCharacter {
public:
    EnemyCharacter(float startX, float startY);
    virtual ~EnemyCharacter() = default;
    void setPosition(float nx, float ny);
    int getSpriteWidth() const { return spriteWidth; }
    int getSpriteHeight() const { return spriteHeight; }
    float getVelocityX() const { return velocityX; }
    void setScale(float sx, float sy);
    void setTexture(const std::string& path);
    virtual void update(float dt, Level& level);
    virtual void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    void fireBullet(PlayerCharacter& p);


protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float velocityY;
    float x, y;
    float velocityX;
    Projectile* projectile;
    bool isfiring;
    int direction;
    float spriteWidth, spriteHeight;
    Animation animWalkR, animWalkL;
    Animation* current;
    void updateDimensions();

};
