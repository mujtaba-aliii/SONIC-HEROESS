#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class collectibles {
public:
    collectibles(float startX, float startY, const std::string& type)
        : x(startX), y(startY), type(type)
    {
        const float scale = 4.0f; // Enlarge 4x for better visibility

        // Load texture based on the type
        if (type == "ring") {
            if (!texture.loadFromFile("Data/ring.png")) {
                std::cerr << "Failed to load texture for ring!" << std::endl;
            }
        }
        else if (type == "lives") {
            if (!texture.loadFromFile("Data/spike.png")) {
                std::cerr << "Failed to load texture for lives!" << std::endl;
            }
        }
        else if (type == "boost") {
            if (!texture.loadFromFile("Data/crabstill.png")) {
                std::cerr << "Failed to load texture for boost!" << std::endl;
            }
        }
        else {
            std::cerr << "Invalid collectible type: " << type << std::endl;
        }

        // Set the texture to the sprite
        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
        sprite.setPosition(x, y);
    }

    int getSpriteWidth() const { return sprite.getTexture() ? sprite.getTexture()->getSize().x : 0; }
    int getSpriteHeight() const { return sprite.getTexture() ? sprite.getTexture()->getSize().y : 0; }

    void render(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    void setScale(float sx, float sy) {
        sprite.setScale(sx, sy);
    }

    void update(float dt, PlayerCharacter* p) {
        collectiblesCollisions(p);
    }

    void setPosition(float nx, float ny) {
        x = nx;
        y = ny;
        sprite.setPosition(x, y);
    }

    void collectiblesCollisions(PlayerCharacter* p) {
        float distance = std::sqrt(std::pow(x - p->getPositionX(), 2.0f) + std::pow(y - p->getPositionY(), 2.0f));

        // Test threshold to detect collisions
        float threshold = 70.0f; // Adjust this value
        if (distance < threshold) {
            setPosition(-100, -100); // Move the collectible off-screen
            std::cout << "Collision detected for type: " << type << std::endl;

            if (type == "boost") {
                // Apply boost effect based on the character type
                if (dynamic_cast<Sonic*>(p)) {
                    // Sonic: Gain +4 speed
                    p->setMaxSpeed(p->getMaxSpeed() + 4.0f);
                    std::cout << "Sonic gained a speed boost! New max speed: " << p->getMaxSpeed() << std::endl;
                }
                else if (dynamic_cast<Tails*>(p)) {
                    // Tails: Gain +4 seconds of flight
                    Tails* tails = dynamic_cast<Tails*>(p);
                    if (tails) {
                        tails->extendFlightTime(4.0f); // Extend flight time
                        std::cout << "Tails gained +4 seconds of flight!" << std::endl;
                    }
                }
                else if (dynamic_cast<Knuckles*>(p)) {
                    // Knuckles: Gain invincibility for 15 seconds
                    Knuckles* knuckles = dynamic_cast<Knuckles*>(p);
                    if (knuckles) {
                        knuckles->activateInvincibility(15.0f); // Activate invincibility
                        std::cout << "Knuckles is now invincible for 15 seconds!" << std::endl;
                    }
                }
            }
        }
    }


protected:
    std::string type; // Type of collectible (e.g., "ring", "lives", "boost")
    sf::Sprite sprite;
    sf::Texture texture;
    float x, y;
};

