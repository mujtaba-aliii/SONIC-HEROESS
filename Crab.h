#pragma once
#include "Animation.h"
#include "Level.h"
#include<iostream>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include<algorithm>

class Crabmeat :public EnemyCharacter {
public:
    Crabmeat(float startX, float startY)
        : x(startX), y(startY), velocityX(1.5f), direction(1), minX(startX - 600), maxX(startX + 600), EnemyCharacter(startX, startY), state("normal")
    {
        const int frameW = 60, frameH = 64;
        const float frameTime = 0.08f;
        const int cropX = 8, cropW = 48;
        const float scale = 2.5f;

        setPosition(x, y);

        animWalkR.load("Data/enemy.png", frameW, frameH, 5, frameTime, cropX, cropW);
        animWalkL.load("Data/enemy.png", frameW, frameH, 5, frameTime, cropX, cropW);

        animWalkR.getSprite().setScale(scale, scale);
        animWalkL.getSprite().setScale(scale, scale);
        animBullet.getSprite().setScale(scale, scale);

        current = &animWalkR;
        current->getSprite().setPosition(x, y);
    }

    sf::Vector2f calculateProjectileVelocity(sf::Vector2f origin, sf::Vector2f target, float speed) {
        sf::Vector2f toTarget = target - origin;
        float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
        if (distance == 0) return { 0.f, 0.f };
        sf::Vector2f direction = toTarget / distance;
        return direction * speed;
    }

    void bullet(const Level& level, PlayerCharacter* p) {
        velocityX = 0;
        if (!hasFired && p) {
            float groundY = (level.getHeight() - 1) * level.getCellSize();
            sf::Vector2f playerPos = { x - 300,groundY - 300 };

            float bulletSpeed = 300.f;
            sf::Vector2f velocity = calculateProjectileVelocity({ x, y }, playerPos, bulletSpeed);

            sf::Vector2f velocity2 = calculateProjectileVelocity({ x, y }, { x + 300,groundY - 300 }, bulletSpeed);
            sf::Vector2f origin = current->getSprite().getPosition();
            for (int i = 0; i < MAX_BULLETS; ++i) {
                if (!bullets[i].active) {
                    bullets[i].init(origin, velocity, "Data/BLAST.png", 28, 48, 2, 0.08f, 0, 28, 2.5f);

                    break;
                }
                if (!bullets2[i].active) {
                    bullets2[i].init(origin, velocity2, "Data/BLAST.png", 28, 48, 2, 0.08f, 0, 28, 2.5f);

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

    void setTexture() {
        if (!texture.loadFromFile("Data/enemy.png")) {
            std::cout << "Failed to load texture: Data/enemy.png" << std::endl;
        }
        sprite.setTexture(texture);
        updateDimensions();
    }

    void update(float dt, Level& level, PlayerCharacter* p) {
        if (state == "normal") {
            x -= velocityX + dt;
            if (x <= minX) {

                direction = -1;
            }
            else if (x >= maxX) {

                direction = 1;
            }
            checkplayer(p);
        }
        else if (state == "attack") {
            if (!hasFired && attackTimer.getElapsedTime().asSeconds() > 1.0f) {
                bullet(level, p);
            }
        }
        else if (state == "returning") {
            if (attackTimer.getElapsedTime().asSeconds() > 1.0f) {
                hasFired = false;
                state = "normal";
                velocityX = 1.5;
                current = &animWalkR;

            }
        }
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                bullets[i].update(dt);
                if (bullets[i].CheckCollisions(p)) {
                    p->decreaseLives(dt);
                }

            }if (bullets2[i].active) {
                bullets2[i].update(dt);
                if (bullets2[i].CheckCollisions(p)) {
                    p->decreaseLives(dt);
                }
            }
        }

        Animation* next = &animWalkR;
        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }
        playerenemiecollision(dt, level, p);

        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);
    }

    void render(sf::RenderWindow& window) {
        window.draw(current->getSprite());
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (bullets[i].active) {
                bullets[i].render(window);
            }
            if (bullets2[i].active) {
                bullets2[i].render(window);
            }

        }

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

                crabHP--;
                if (crabHP < 0) {
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
private:
    float x, y;
    float velocityX = 2.0f;
    int direction;
    float minX, maxX;
    std::string state;
    bool hasFired = false;
    int crabHP = 4;
    static const int MAX_BULLETS = 10;
    Bullet bullets[MAX_BULLETS];
    Bullet bullets2[MAX_BULLETS];
    sf::Clock attackTimer;
    Animation animWalkR, animWalkL, animBullet;
    Animation* current;
};
