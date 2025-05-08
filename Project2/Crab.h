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
        : x(startX), y(startY), velocityX(1.5f), direction(1), EnemyCharacter(startX, startY)
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

        current = &animWalkR;
        current->getSprite().setPosition(x, y);
    }
    void setTexture() {
        if (!texture.loadFromFile("Data/enemy.png")) {
            std::cout << "Failed to load texture: Data/enemy.png" << std::endl;
        }
        sprite.setTexture(texture);
        updateDimensions();
    }
    //    void update(float dt, const Level& level) {
    //        float nextX = x - velocityX * direction;
    //        float footY = y + getSpriteHeight() - 1;
    //float checkX = (direction > 0) ? nextX : nextX + getSpriteWidth() - 1;
    //
    //bool collisionAhead = level.collidesAt(checkX, footY);
    //        std::cout << "Hellooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo" << collisionAhead << std::endl;
    //        if (collisionAhead) {
    //            direction *= -1; // Turn around
    //        }
    //        else {
    //            x = nextX;
    //        }
    //
    //        // Update sprite position
    //        sprite.setPosition(x, y);
    //
    //        // Update animation frame if needed
    //       current->update(dt);
    //       current->getSprite().setPosition(x, y);
    //    }

    void update(float dt, const Level& level) {
        //x-=  velocityX * direction * dt * 30.f;
        // /*sf::FloatRect bounds = current->getSprite().getGlobalBounds();
        // float footY = y + bounds.height - 2;
        // float leftFootX = x + 8;
        // float rightFootX = x + bounds.width - 8;
        // float margin = 2.0f;
        // bool collL = level.collidesAt(leftFootX-margin, footY);
        // bool collR = level.collidesAt(rightFootX+margin, footY);

        // std::cout << "Hellooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo" << collL <<"HIIIIIIIIIIIIIIIIIIIIII"<<collR << std::endl;
        // if (collL && direction < 0 || collR && direction > 0) {
        //     direction = -direction;
        //     current = (direction > 0) ? &animWalkR : &animWalkL;
        //     current->reset();
        // }*/
        //float probeX = (direction < 0) ? x - 2.f : x + spriteWidth + 2.f;
        //float probeYTop = y + 5.f; // a bit above feet
        //float probeYBottom = y + spriteHeight - 2.f;

        //bool wallAhead = level.collidesAt(probeX, probeYTop) || level.collidesAt(probeX, probeYBottom);
        //if (wallAhead) {
        //    direction = -direction;
        //    current = (direction > 0) ? &animWalkR : &animWalkL;
        //    current->reset();
        //}
 // Predict crab's next horizontal position
        x -= velocityX * direction * dt * 60.f;

        Animation* next = &animWalkR;
        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }


        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);
    }

    void render(sf::RenderWindow& window) {
        window.draw(current->getSprite());
    }

private:
    float x, y;
    float velocityX = 2.0f;
    int direction;

    Animation animWalkR, animWalkL;
    Animation* current;
};

//#include "Enemy.h"
//#include "Animation.h"
//#include <iostream>
//class Crab {
//public:
//    Crab(float startX, float startY) {
//    }
//    virtual ~Crab() ;
//
//    
//    virtual void update(float dt, Level& level);
//    virtual void render(sf::RenderWindow& window);
//
//protected:
//    Animation animIdleR, animRunR, animJumpR, animHang;
//    Animation animIdleL, animRunL, animJumpL, animHangL;
//    Animation* current = nullptr;
//};

