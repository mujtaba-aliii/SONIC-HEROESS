#pragma once
#include "Animation.h"
#include "Level.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Boss : public EnemyCharacter {
public:
    Boss(float startX, float startY)
        : x(startX), y(startY), velocityX(1.5f), direction(1), EnemyCharacter(startX, startY), minX(startX - 200), maxX(startX + 200)
    {
        const int frameW = 54, frameH = 64;
        const float frameTime = 0.09f;
        const int cropX = 8, cropW = 48;
        const float scale = 2.5f;

        setPosition(x, y);

        animWalkR.load("Data/Bossn.png", frameW, frameH, 2, frameTime, cropX, cropW);
        animWalkL.load("Data/Boss1.png", frameW, frameH, 2, frameTime, cropX, cropW);

        animAttackR.load("Data/Bossattack.png", frameW, frameH, 2, frameTime, cropX, cropW);
        animattackL.load("Data/Boss_attack2.png", frameW, frameH, 2, frameTime, cropX, cropW);

        animWalkR.getSprite().setScale(scale, scale);
        animWalkL.getSprite().setScale(scale, scale);

        animAttackR.getSprite().setScale(scale, scale);
        animattackL.getSprite().setScale(scale, scale);

        current = &animWalkR;
        current->getSprite().setPosition(x, y);
    }
    void setTexture() {
        if (!texture.loadFromFile("Data/Bossn.png")) {
            std::cout << "Failed to load texture: Data/MotoBug1.png" << std::endl;
        }
        if (!texture.loadFromFile("Data/Boss1.png")) {
            std::cout << "Failed to load texture: Data/MotoBug2.png" << std::endl;
        }
        if (!texture.loadFromFile("Data/Bossattack.png")) {
            std::cout << "Failed to load texture: Data/MotoBug2.png" << std::endl;
        }
        if (!texture.loadFromFile("Data/Bossattack2.png")) {
            std::cout << "Failed to load texture: Data/MotoBug2.png" << std::endl;
        }
        sprite.setTexture(texture);
        updateDimensions();
    }
    void update(float dt, const Level& level, PlayerCharacter* p) {


        x += velocityX * direction + dt;
        std::cout << x << std::endl;
        if (x >= maxX) {
            direction = 1;
        }
        if (x <= minX) {
            direction = -1;
        }
        current = (direction > 0) ? &animWalkR : &animWalkL;
        Animation* next = &animWalkR;
        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }
        if (CheckCollisions(p)) {
            p->decreaseLives(dt);
        }
        float groundY = (level.getHeight() - 1) * level.getCellSize();
        if (x == p->getPositionX()) {
            while (y != groundY) {
                current = (direction > 0) ? &animAttackR : &animattackL;
                y += 50;
            }
        }
        playerenemiecollision(dt, level, p);
        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);

    }
    void playerenemiecollision(float dt, const Level& level, PlayerCharacter* p) {
        float PosX = current->getSprite().getPosition().x;
        float PosY = current->getSprite().getPosition().y;
        float playerX = p->getPositionX();
        float playerY = p->getPositionY();

        float dx = playerX - PosX;
        float dy = playerY - PosY;
        float distance = std::sqrt(dx * dx + dy * dy);
        float threshold = 80.0f;

        if (distance < threshold) {
            float dvx = p->getVelocityX() - velocityX;
            float dvy = p->getVelocityY() - velocityY;
            float dot = dx * dvx + dy * dvy;

            if (dot < 0) {
                motoHP--;
                if (motoHP < 0) {
                    x = -100;
                    y = -100;
                    setPosition(x, y);

                }

                float directionX = playerX - PosX;
                float directionY = playerY - PosY;
                float length = std::sqrt(directionX * directionX + directionY * directionY);
                if (length != 0) direction /= length;
                p->setVelocity(directionX * 30.f, -std::abs(directionY * 30.f));
            }
        }
    }
    bool CheckCollisions(PlayerCharacter* p) {

        float distance = std::sqrt(std::pow(x - p->getPositionX(), 2.0f) + std::pow(y - p->getPositionY(), 2.0f));

        float threshold = 150.0f;
        if (distance < threshold) {
            return true;
        }
        else
            return false;
    }
    void render(sf::RenderWindow* window) {
        window->draw(current->getSprite());
    }

private:
    float x, y;
    float velocityX = 1.5f;
    int direction;
    int motoHP = 2;
    float minX;
    float maxX;
    Animation animWalkR, animWalkL;
    Animation* current;
    Animation animAttackR, animattackL;
};

