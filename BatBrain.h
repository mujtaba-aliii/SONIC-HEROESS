#pragma once
#include "Animation.h"
#include "Level.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
class BatBrain : public EnemyCharacter {
public:
    BatBrain(float startX, float startY)
        : EnemyCharacter(startX, startY), x(startX), y(startY), originalX(startX), originalY(startY), velocityX(1.5f), direction(1),
        isFlying(false), flyingDelay(3.f), numFrames(9), currentFrame(0), timeSinceLastFrame(0.f), frameTime(0.08f), state("still"), minX(startX - 600), maxX(startX + 600)
    {

        animWalkR.load("Data/bb.png", 45, 41, 3, 0.08f, 0, 0);
        animWalkL.load("Data/bbL.png", 45, 41, 3, 0.08f, 0, 0);
        animStill.load("Data/bat_still.png", 25, 64, 1, 0.08f, 0, 0);

        float scale = 2.5;
        animWalkR.getSprite().setScale(scale, scale);
        animWalkL.getSprite().setScale(scale, scale);
        animAttack.getSprite().setScale(scale, scale);
        animBullet.getSprite().setScale(scale, scale);
        current = &animStill;


        current->getSprite().setPosition(x, y);

        flyTimer.restart();
    }





    void update(float dt, const Level& level, PlayerCharacter* p) {
        timeSinceLastFrame += dt;

        if (state == "still") {
            velocityX = 0;
            checkplayer(p);
        }
        else if (state == "attack") {

            move(dt, level, p);
        }
        else if (state == "returning") {

            moveback(dt, level, p);
        }

        if (CheckCollisions(p)) {
            p->decreaseLives(dt);
        }
        playerenemiecollision(dt, level, p);
        current->update(dt);
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
                batHP--;
                if (batHP < 0) {
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


    sf::Vector2f calculateProjectileVelocity(sf::Vector2f origin, sf::Vector2f target, float speed) {
        sf::Vector2f toTarget = target - origin;
        float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
        if (distance == 0) return { 0.f, 0.f };
        sf::Vector2f direction = toTarget / distance;
        return direction * speed;
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
    void moveback(float dt, const Level& level, PlayerCharacter* p) {

        sf::Vector2f originalPos = { originalX,originalY };
        velocity = calculateProjectileVelocity({ x, y }, originalPos, 30.f);


        float direction = 1;
        x += velocity.x * dt;
        y += velocity.y + dt;

        if (p->getPositionX() <= minX) {
            direction = -1;

        }
        else if (p->getPositionX() >= maxX) {
            direction = 1;

        }
        current = (direction > 0) ? &animWalkR : &animWalkL;
        current->getSprite().setPosition(x, y);
        float dx = x - originalX;
        float dy = y - originalY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 10.f) { 
            hasFired = false;
            velocity = { 0.f, 0.f };
            state = "still";
            current = &animStill;
            x = originalX;
            y = originalY;
        }


        checkplayer(p);

    }
    void move(float dt, const Level& level, PlayerCharacter* p) {

        sf::Vector2f playerPos = p->getPosition();
        velocity = calculateProjectileVelocity({ x, y }, playerPos, 30.f);


        float direction = 1;
        x += velocity.x * dt;
        y += velocity.y * dt;
        if (x > p->getPositionX()) {
            direction = 1;

        }
        else {
            direction = -1;

        }
        current = (direction > 0) ? &animWalkR : &animWalkL;
        current->getSprite().setPosition(x, y);
        if (p->getPositionX() >= maxX || p->getPositionX() <= minX) {
            state = "returning";
        }

    }



    void checkplayer(PlayerCharacter* p) {
        if (!p) return;


        if (p->getPositionX() >= minX && p->getPositionX() <= maxX) {

            state = "attack";
            attackTimer.restart();
        }
        else if (x > maxX || x < minX) {
            state = "returning";
        }
    }
    void render(sf::RenderWindow& window) {
        if (direction == -1)
            current->getSprite().setScale(-2.5f, 2.5f);
        else
            current->getSprite().setScale(2.5f, 2.5f);
        window.draw(current->getSprite());
    }

private:
    float x, y;
    float velocityX;
    int direction;
    bool isFlying;
    float flyingDelay;
    sf::Clock flyTimer;
    Animation animAttack;
    sf::Texture textures[9];
    sf::Sprite sprites[9];
    sf::Texture stilltextures;
    sf::Sprite stillsprites;
    int currentFrame;
    float frameTime;
    float timeSinceLastFrame;
    std::string state;
    sf::Clock  attackTimer;
    int numFrames;
    bool hasFired = false;
    sf::Vector2f velocity;
    sf::Vector2f position;
    float gravity;
    bool active;
    sf::Sprite sprite;
    Animation animWalkR, animStill, animBullet;
    Animation* current;
    float minX;
    float maxX;
    float originalX;
    float originalY;
    int batHP = 3;
};
