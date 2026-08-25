#pragma once
#include "Animation.h"
#include "Level.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class BuzzBomber : public EnemyCharacter {
public:
    BuzzBomber(float startX, float startY)
        : x(startX), y(startY), originalX(startX), originalY(startY),
        hasFired(false), velocityX(3.0f), direction(1), state("normal"),
        EnemyCharacter(startX, startY)
    {
        const int frameW = 26, frameH = 64;
        const float frameTime = 0.08f;
        const int cropX = 0, cropW = 0;
        const float scale = 2.5f;

        animWalkR.load("Data/buzzbomber1.png", frameW, frameH, 4, frameTime, cropX, cropW);
        animWalkL.load("Data/buzzbomber2.png", frameW, frameH, 4, frameTime, cropX, cropW);
        animAttackR.load("Data/buzzbomber.png", frameW, frameH, 4, frameTime, cropX, cropW);
        animAttackL.load("Data/buzzbomber6.png", frameW, frameH, 4, frameTime, cropX, cropW);
        animBullet.load("Data/bomb.png", frameW, frameH, 4, frameTime, cropX, cropW);

        if (!bulletTexture.loadFromFile("Data/bomb.png")) {
            std::cerr << "Failed to load bullet texture.\n";
        }

        animWalkR.getSprite().setScale(scale, scale);
        animWalkL.getSprite().setScale(scale, scale);
        animAttackR.getSprite().setScale(scale, scale);
        animAttackL.getSprite().setScale(scale, scale);
        animBullet.getSprite().setScale(scale, scale);

        current = (direction < 0) ? &animWalkL : &animWalkR;
        current->getSprite().setPosition(x, y);
    }

    void update(float dt, const Level& level, PlayerCharacter* p, sf::RenderWindow* window) {
        float maxX = 0;
        float maxY = 0;

        for (int i = 0; i < level.getHeight(); ++i) {
            for (int j = 0; j < level.getWidth(); ++j) {
                if (level.getGridCell(i,j) == 'w') {
                    float x = j * level.getCellSize();
                    float y = i * level.getCellSize();
                    if (x > maxX) maxX = x;
                    if (y > maxY) maxY = y;
                }
            }
        }

        float levelWidth = maxX + level.getCellSize(); 
        float levelHeight = maxY + level.getCellSize();

        if (state == "normal") {
            x -= velocityX * direction + dt;
            const float frameWidth = 54.f;  
            const float scaleX = 2.5f;
            const float spriteWidth = frameWidth * scaleX;
            float groundY = (level.getHeight() - 1) * level.getCellSize();
            float distanceToGround = groundY - (y + current->getSprite().getGlobalBounds().height);
            
                if (distanceToGround > 50.f) {
                    if (x + spriteWidth * 0.8 >= levelWidth) {

                        y += 50;
                        direction = 1;
                        current = &animWalkR;
                        

                    }

                    if (x <= 0) {
                        y += 50;
                        direction = -1;
                        current = &animWalkL;
                        
                    }
                }
            if (attackTimer.getElapsedTime().asSeconds() > shoot_time) {
                state = "attack";
                attackTimer.restart();
            }
        }
        else if (state == "attack") {
            if (!hasFired && attackTimer.getElapsedTime().asSeconds() > shoot_time) {
                bullet(level, p);
            }
        }
        else if (state == "returning") {
            if (attackTimer.getElapsedTime().asSeconds() > 1.0f) {
                hasFired = false;
                state = "normal";
                velocityX = 1.5;
                current = direction < 0 ? &animWalkL : &animWalkR;
                setPosition(originalX, originalY);
            }
        }
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                bullets[i].update(dt);
                if (bullets[i].CheckCollisions(p)) {
                    p->decreaseLives(dt);
                }
            }
        }
        playerenemiecollision(dt, level, p);
        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);
    }

    sf::Vector2f calculateProjectileVelocity(sf::Vector2f origin, sf::Vector2f target, float speed) {
        sf::Vector2f toTarget = target - origin;
        float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
        if (distance == 0) return { 0.f, 0.f };
        sf::Vector2f direction = toTarget / distance;
        return direction * speed;
    }
    void playerenemiecollision(float dt, const Level& level, PlayerCharacter* p) {
        sf::Vector2f Pos = current->getSprite().getPosition();
        sf::Vector2f playerPos(p->getPositionX(), p->getPositionY());

        float dx = playerPos.x - Pos.x;
        float dy = playerPos.y - Pos.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float threshold = 80.0f;

        if (distance < threshold) {
            float dvx = p->getVelocityX() - velocityX;
            float dvy = p->getVelocityY() - velocityY;
            float dot = dx * dvx + dy * dvy;

            if (dot < 0) {
                
                    buzzHP--;
                if (buzzHP < 0) {
                    x = -100;
                    y = -100;
                    setPosition(x, y);

                }
                sf::Vector2f direction = playerPos - Pos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) direction /= length;
                p->setVelocity(direction.x * 30.f, -std::abs(direction.y * 30.f));
            }
        }
    }
    void bullet(const Level& level, PlayerCharacter* p) {
        velocityX = 0;
        if (!hasFired && p) {
            float groundY = (level.getHeight() - 1) * level.getCellSize();
            sf::Vector2f playerPos = { x - 300 * direction,groundY };
            current = (direction < 0) ? &animAttackL : &animAttackR;
            current->getSprite().setPosition(x, y);
            float bulletSpeed = 300.f;
            sf::Vector2f velocity = calculateProjectileVelocity({ x, y }, playerPos, bulletSpeed);
            sf::Vector2f origin = current->getSprite().getPosition();
            for (int i = 0; i < MAX_BULLETS; ++i) {
                if (!bullets[i].active) {
                    bullets[i].init(origin, velocity, "Data/shooot.png", 24, 34, 2, 0.1f, 0, 24, 2.5f);
                    break;
                }
            }

            hasFired = true;
            attackTimer.restart();
            state = "returning";
        }
    }

    void checkplayer(PlayerCharacter* p) {
        if (!p) return;

        float dx = x - p->getPositionX();
        float dy = y - p->getPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        float detectionRange;
        if (y < 500)
            detectionRange = y + 300.0f;
        else
            detectionRange = y - 150;

        if (distance <= detectionRange) {
            state = "attack";
            attackTimer.restart();
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(current->getSprite());

        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                bullets[i].render(window);
            }
        }

    }

private:
    float x, y;
    float originalX, originalY;
    float velocityX;
    int direction;
    bool hasFired;
    float shoot_time = 5.0f;

    std::string state;
    sf::Clock attackTimer;

    static const int MAX_BULLETS = 10;
    Bullet bullets[MAX_BULLETS];
    sf::Texture bulletTexture;
    int buzzHP = 5;
    Animation animWalkR, animWalkL, animAttackR, animAttackL, animBullet;
    Animation* current;
    Animation* current1;
};