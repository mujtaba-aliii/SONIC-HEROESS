#pragma once
#include "Animation.h"
#include "Level.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
//class BatBrain :public EnemyCharacter {
//public:
//    BatBrain(float startX, float startY) : x(startX), y(startY), velocityX(1.5f), direction(1), EnemyCharacter(startX, startY)
//    {
//        const int frameW = 25, frameH = 64;
//        const float frameTime = 0.08f;
//        const int cropX = 0, cropW = 0;
//        const float scale = 2.5f;
//
//        setPosition(x, y);
//
//        animWalkR.load("Data/buzzbomber1.png", frameW, frameH, 4, frameTime, cropX, cropW);
//
//        animWalkR.getSprite().setScale(scale, scale);
//        animWalkL.getSprite().setScale(scale, scale);
//
//        current = &animWalkR;
//        current->getSprite().setPosition(x, y);
//    }
//    void setTexture() {
//        if (!texture.loadFromFile("Data/buzzbomber1.png")) {
//            std::cout << "Failed to load texture: Data/MotoBug1.png" << std::endl;
//        }
//
//        sprite.setTexture(texture);
//        updateDimensions();
//    }
//    void update(float dt, const Level& level) {
//
//
//        x -= velocityX + dt;
//
//        // Switch direction if needed (you can add collision logic)
//      /*  if (x < level.getCellSize()) direction = 1;
//        if (x > level.getWidth() * level.getCellSize() - getSpriteWidth()) direction = -1;*/
//
//        // Choose animation
//        // Animation next = (direction == 1) ? &animWalkR : &animWalkL;*/
//        Animation* next = &animWalkR;
//        if (next != current) {
//            current->stop();
//            current = next;
//            current->reset();
//        }
//
//        current->play();
//        current->update(dt);
//        current->getSprite().setPosition(x, y);
//        std::cout << "                                          CRAB" << x << " " << y << std::endl;
//    }
//    void bullet(float dt, Level& level, PlayerCharacter& p) {
//
//    }
//    void render(sf::RenderWindow& window) {
//        window.draw(current->getSprite());
//    }
//
//private:
//    float x, y;
//    float velocityX = 1.5f;
//    int direction;
//
//    Animation animWalkR, animWalkL;
//    Animation* current;
//};
class BatBrain : public EnemyCharacter {
public:
    BatBrain(float startX, float startY)
        : EnemyCharacter(startX, startY), x(startX), y(startY), velocityX(1.5f), direction(1),
        isFlying(false), flyingDelay(3.f), numFrames(9), currentFrame(0), timeSinceLastFrame(0.f), frameTime(0.08f)
    {
        // Initialize the texture and sprite array
        for (int i = 0; i < numFrames; i++) {
            if (!textures[i].loadFromFile("Data/batbrain-" + std::to_string(i + 1) + ".png")) {
                std::cout << "Failed to load texture for frame " << i + 1 << std::endl;
            }
            sprites[i].setTexture(textures[i]);
            sprites[i].setScale(2.5f, 2.5f); // Adjust scale as needed
        }

        // Set initial position
        setPosition(x, y);

        // Initialize timer for delay before flying
        flyTimer.restart();  // Start the timer immediately
    }

    void update(float dt, const Level& level) {
        x -= velocityX + dt;
        timeSinceLastFrame += dt;

        // If enough time has passed, move to the next frame
        if (timeSinceLastFrame >= frameTime) {
            timeSinceLastFrame = 0.f;
            currentFrame = (currentFrame + 1) % numFrames; // Loop through the frames
        }

        // Check if the BatBrain should start flying
        if (!isFlying && flyTimer.getElapsedTime().asSeconds() >= flyingDelay) {
            isFlying = true;  // Start flying after the delay
        }

        // Update position if flying
        if (isFlying) {
            x -= velocityX * dt; // Move the bat
        }

        // Choose the correct sprite (flip the sprite based on direction)
        if (direction == -1) {
            sprites[currentFrame].setScale(-2.5f, 2.5f); // Flip for left movement
        }
        else {
            sprites[currentFrame].setScale(2.5f, 2.5f); // Right facing sprite
        }

        // Update the sprite's position on screen
        sprites[currentFrame].setPosition(x, y);

        // Optional: Collision checks and direction changes (add your logic here)
        if (x < level.getCellSize()) direction = 1;
        if (x > level.getWidth() * level.getCellSize() - getSpriteWidth()) direction = -1;
    }

    void render(sf::RenderWindow& window) {
        window.draw(sprites[currentFrame]); // Draw the current frame of the animation
    }

private:
    float x, y;
    float velocityX;
    int direction;
    bool isFlying; // Flag to track if BatBrain should start flying
    float flyingDelay; // Delay before BatBrain starts flying
    sf::Clock flyTimer; // Timer to track flying delay

    sf::Texture textures[9]; // Array to store 9 textures
    sf::Sprite sprites[9]; // Array to store the corresponding sprites
    int currentFrame; // Tracks the current frame in the animation
    float frameTime; // Time per frame (controls the speed of animation)
    float timeSinceLastFrame; // Time tracker for frame updates

    int numFrames; // Total number of frames in the animation
};
