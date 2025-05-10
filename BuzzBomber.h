#pragma once
#include "Animation.h"
#include "Level.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
class BuzzBomber :public EnemyCharacter {
public:
    BuzzBomber(float startX, float startY) : x(startX), y(startY), velocityX(1.5f), direction(1), EnemyCharacter(startX, startY)
    {
        const int frameW = 25, frameH = 64;
        const float frameTime = 0.08f;
        const int cropX = 0, cropW = 0;
        const float scale = 2.5f;

        setPosition(x, y);

        animWalkR.load("Data/buzzbomber1.png", frameW, frameH, 4, frameTime, cropX, cropW);

        animWalkR.getSprite().setScale(scale, scale);
        animWalkL.getSprite().setScale(scale, scale);

        current = &animWalkR;
        current->getSprite().setPosition(x, y);
    }
    void setTexture() {
        if (!texture.loadFromFile("Data/buzzbomber1.png")) {
            std::cout << "Failed to load texture: Data/MotoBug1.png" << std::endl;
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
        // Animation next = (direction == 1) ? &animWalkR : &animWalkL;*/
            Animation * next = &animWalkR;
        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }

        current->play();
        current->update(dt);
        current->getSprite().setPosition(x, y);
        std::cout << "                                          CRAB" << x << " " << y << std::endl;
    }
    void bullet(float dt, Level& level, PlayerCharacter& p) {

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
//class BuzzBomber : public EnemyCharacter {
//public:
//    BuzzBomber(float startX, float startY)
//        : x(startX), y(startY), velocityX(1.5f), direction(1),
//        projectile(startX, startY), isFiring(false), 
//        EnemyCharacter(startX, startY)
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
//        animAttack.load("Data/buzzbomber.png", frameW, frameH, 4, frameTime, cropX, cropW);
//        animWalkR.getSprite().setScale(scale, scale);
//        animAttack.getSprite().setScale(scale, scale);
//
//        current = &animWalkR;
//        current->getSprite().setPosition(x, y);
//
//        
//    }
//    void update(float dt, const Level& level, PlayerCharacter* p) {
//        // Calculate the distance between the BuzzBomber and the player
//        float playerDist = std::abs(p->getPositionX() - x);
//
//        if (playerDist < 80) {
//            velocityX = 0;  
//
//            Animation* next = &animAttack; 
//            if (next != current) {
//                current->stop();
//                current = next;
//                current->reset();
//            }
//
//            // Implement the attack logic (shoot projectiles or trigger an attack animation)
//            isFiring = true;  // You can use this flag to start firing projectiles
//        }
//        else {
//            // Continue walking if the player is not in range
//            velocityX = 1.5f;  // Restore walking speed
//
//            // Switch back to the walking animation
//            Animation* next = &animWalkR;  // You can add left-right switching if needed
//            if (next != current) {
//                current->stop();
//                current = next;
//                current->reset();
//            }
//
//            isFiring = false;  // Stop firing if the player is not in range
//        }
//
//        // Continue animating and updating the sprite position
//        current->play();
//        current->update(dt);
//        current->getSprite().setPosition(x, y);
//
//        std::cout << "BuzzBomber Position: " << x << ", " << y << std::endl;
//    }
//
//    void render(sf::RenderWindow& window) {
//        window.draw(current->getSprite());
//
//        if (isFiring) {
//            
//            window.draw(shootSprite);  // Draw the bullet sprite
//        }
//    }

  //  void update(float dt, const Level& level, PlayerCharacter* p) {
  //      x -= velocityX + dt;

  //      // Handle animation switching (if needed)
  //      Animation* next = &animWalkR; // You can add direction-based switching if needed
  //      if (next != current) {
  //          current->stop();
  //          current = next;
  //          current->reset();
  //      }

  //      current->play();
  //      current->update(dt);
  //      current->getSprite().setPosition(x, y);
  //      /*if (p->getPositionX() - x < 80 && !isFiring) {
  //          isFiring = true;
  //          current->getSprite().setPosition(13000, -10000);
  //      }
		//*/
  //      float playerDist = std::abs(p->getPositionX() - x);
  //      //if (playerDist < 10 && !isFiring) {
  //      //    isFiring = true;
  //      //    current->getSprite().setPosition(13000, -10000);  // Move the sprite off-screen
  //      //}
  //      if (playerDist < 80) {
  //         
  //          current->getSprite().setColor(sf::Color(255, 255, 255, 0)); 
  //      }
  //      else {
  //          
  //          current->getSprite().setColor(sf::Color(255, 255, 255, 255)); 
  //      }
  //      std::cout << "BuzzBomber Position: " << x << ", " << y << std::endl;

  //      std::cout << "BuzzBomber Position: " << x << ", " << y << std::endl;
  //  }



//private:
//    float x, y;
//    float velocityX;
//    int direction;
//
//    Animation animWalkR, animWalkL,animAttack;
//    Animation* current;
//
//    Projectile projectile;
//    bool isFiring=false;
//
//    sf::Texture shootTexture;
//    sf::Sprite shootSprite;
//
//};
//
//#pragma once
//#include "Animation.h"
//#include "Level.h"
//#include "Enemy.h"
//#include "PlayerCharacter.h"
//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <cmath>
//
//class BuzzBomber : public EnemyCharacter {
//public:
//    BuzzBomber(float startX, float startY)
//        : x(startX), y(startY), velocityX(1.5f), direction(1),
//        isFiring(false), bulletCooldown(2.0f), timeSinceLastShot(0.0f),
//        bulletSpeed(2.0f), shootX(0), shootY(0), bulletVx(0), bulletVy(0),
//        EnemyCharacter(startX, startY) {
//        const int frameW = 25, frameH = 64;
//        const float frameTime = 0.08f;
//        const float scale = 2.5f;
//
//        setPosition(x, y);
//
//        animWalkR.load("Data/buzzbomber1.png", frameW, frameH, 4, frameTime, 0, 0);
//        animWalkR.getSprite().setScale(scale, scale);
//        animWalkL.getSprite().setScale(scale, scale);
//
//        current = &animWalkR;
//        current->getSprite().setPosition(x, y);
//
//        // Load the shoot texture for the bullet
//        if (!shootTexture.loadFromFile("Data/shoot.png")) {
//            std::cout << "Failed to load shoot.png" << std::endl;
//        }
//        else {
//            std::cout << "Successfully loaded shoot.png!" << std::endl;
//        }
//        shootSprite.setTexture(shootTexture);
//        shootSprite.setScale(0.5f, 0.5f);  // Adjust scale if needed
//    }
//
//    void update(float dt, const Level& level, PlayerCharacter* p) {
//        // Move the enemy
//        x -= velocityX + dt;
//        current->getSprite().setPosition(x, y);
//
//        // Animation control
//        Animation* next = &animWalkR;
//        if (next != current) {
//            current->stop();
//            current = next;
//            current->reset();
//        }
//
//        current->play();
//        current->update(dt);
//
//        // Shooting logic
//        timeSinceLastShot += dt;
//        float playerDist = std::abs(p->getPositionX() - x);
//
//        if (playerDist < 150 && timeSinceLastShot >= bulletCooldown) {
//            isFiring = true;
//            shootX = x;
//            shootY = y;
//
//            // Set direction toward the player
//            float dx = p->getPositionX() - shootX;
//            float dy = p->getPositionY() - shootY;
//            float length = std::sqrt(dx * dx + dy * dy);
//            if (length != 0) {
//                bulletVx = bulletSpeed * (dx / length);
//                bulletVy = bulletSpeed * (dy / length);
//            }
//
//            timeSinceLastShot = 0.0f;
//        }
//
//        if (isFiring) {
//            // Update bullet position
//            shootX += bulletVx * dt; // Make sure to multiply by dt for smooth movement
//            shootY += bulletVy * dt;
//
//            // Optional: reset or stop firing when the bullet goes off-screen
//            if (shootX < 0 || shootX > level.getWidth() || shootY < 0 || shootY > level.getHeight()) {
//                isFiring = false; // Stop firing when out of bounds
//            }
//        }
//
//        std::cout << "BuzzBomber Position: " << x << ", " << y << std::endl;
//    }
//
//    void render(sf::RenderWindow& window) {
//        window.draw(current->getSprite());
//
//        if (isFiring) {
//            // Update the bullet's position
//            shootSprite.setPosition(shootX, shootY);
//            window.draw(shootSprite);  // Draw the bullet sprite
//        }
//    }
//
//private:
//    float x, y;
//    float velocityX;
//    int direction;
//
//    Animation animWalkR, animWalkL;
//    Animation* current;
//
//    bool isFiring;
//    float bulletCooldown;
//    float timeSinceLastShot;
//    float bulletSpeed;
//    float shootX, shootY;
//    float bulletVx, bulletVy;
//
//    sf::Texture shootTexture;  // Texture for the bullet
//    sf::Sprite shootSprite;    // Sprite for the bullet
//};
