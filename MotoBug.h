
#pragma once
#include "Animation.h"
#include "Level.h"
#include<iostream>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include<algorithm>
class MotoBug :public EnemyCharacter {
public:
    MotoBug(float startX, float startY)
        : x(startX), y(startY), velocityX(1.5f), direction(1), EnemyCharacter(startX, startY)
    {
        const int frameW = 54, frameH = 64;
        const float frameTime = 0.09f;
        const int cropX = 8, cropW = 48;
        const float scale = 2.5f;

        setPosition(x, y);

        animWalkR.load("Data/MotoBug1.png", frameW, frameH, 2, frameTime, cropX, cropW);
        animWalkL.load("Data/MotoBug2.png", frameW, frameH, 2, frameTime, cropX, cropW);

        animWalkR.getSprite().setScale(scale, scale);
        animWalkL.getSprite().setScale(scale, scale);

        current = &animWalkR;
        current->getSprite().setPosition(x, y);
    }
    void setTexture() {
        if (!texture.loadFromFile("Data/MotoBug1.png")) {
            std::cout << "Failed to load texture: Data/MotoBug1.png" << std::endl;
        }
        if (!texture.loadFromFile("Data/MotoBug2.png")) {
            std::cout << "Failed to load texture: Data/MotoBug2.png" << std::endl;
        }
        sprite.setTexture(texture);
        updateDimensions();
    }
    void update(float dt, const Level& level, PlayerCharacter* p) {


        x -= velocityX + dt;

        Animation* next = &animWalkR;
        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }
        if (CheckCollisions(p)) {
            p->decreaseLives(dt);
        }

        playerenemiecollision(dt, level, p);
        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);

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

                motoHP--;
                if (motoHP < 0) {
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
    bool CheckCollisions(PlayerCharacter* p) {

        float distance = std::sqrt(std::pow(x - p->getPositionX(), 2.0f) + std::pow(y - p->getPositionY(), 2.0f));

        float threshold = 150.0f;
        if (distance < threshold) {
            return true;
        }
        else
            return false;
    }
    void render(sf::RenderWindow& window) {
        window.draw(current->getSprite());
    }

private:
    float x, y;
    float velocityX = 1.5f;
    int direction;
    int motoHP = 2;
    Animation animWalkR, animWalkL;
    Animation* current;
};
