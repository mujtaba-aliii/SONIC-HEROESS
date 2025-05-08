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
    void update(float dt, const Level& level) {


        x -= velocityX + dt;

        // Switch direction if needed (you can add collision logic)
      /*  if (x < level.getCellSize()) direction = 1;
        if (x > level.getWidth() * level.getCellSize() - getSpriteWidth()) direction = -1;*/

        // Choose animation
        /*Animation* next = (direction == 1) ? &animWalkR : &animWalkL;*/
        Animation* next = &animWalkR;
        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }

        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);
        //std::cout << "                                          CRAB" << x << " " << y << std::endl;
    }

    void render(sf::RenderWindow& window) {
        window.draw(current->getSprite());
    }

private:
    float x, y;
    float velocityX = 1.5f;
    int direction;

    Animation animWalkR, animWalkL;
    Animation* current;
};
