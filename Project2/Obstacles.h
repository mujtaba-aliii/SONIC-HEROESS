#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "PlayerCharacter.h" // Include PlayerCharacter for collision handling

class obstacles {
public:
    // Constructor to initialize the obstacle
    obstacles(float startX, float startY, const std::string& type, int cellSize)
        : x(startX), y(startY), type(type), hasDamagedPlayer(false)
    {
        // Load texture based on the type
        if (type == "spike") {
            if (!texture.loadFromFile("Data/spike.png")) {
                std::cerr << "Failed to load texture for spike!" << std::endl;
            }
        }
        else if (type == "platform") {
            if (!texture.loadFromFile("Data/brick2.png")) {
                std::cerr << "Failed to load texture for platform!" << std::endl;
            }
        }
        else {
            std::cerr << "Invalid obstacle type: " << type << std::endl;
        }

        // Set the texture to the sprite
        sprite.setTexture(texture);

        // Scale the sprite to match the cell size
        sf::Vector2u textureSize = texture.getSize();
        float scaleX = float(cellSize) / textureSize.x;
        float scaleY = float(cellSize) / textureSize.y;
        sprite.setScale(scaleX, scaleY);

        sprite.setPosition(x, y);
    }

    // Get the width of the sprite
    int getSpriteWidth() const {
        return sprite.getTexture() ? sprite.getTexture()->getSize().x : 0;
    }

    // Get the height of the sprite
    int getSpriteHeight() const {
        return sprite.getTexture() ? sprite.getTexture()->getSize().y : 0;
    }

    // Render the obstacle on the window
    void render(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    // Set the scale of the obstacle
    void setScale(float sx, float sy) {
        sprite.setScale(sx, sy);
    }

    // Update the obstacle (e.g., check for collisions)
    void update(float dt, PlayerCharacter* p) {
        handleCollision(p);
    }

    // Set the position of the obstacle
    void setPosition(float nx, float ny) {
        x = nx;
        y = ny;
        sprite.setPosition(x, y);
    }

protected:
    // Handle collisions for all obstacle types
    void handleCollision(PlayerCharacter* p) {
        // Get player and obstacle positions
        float playerBottomY = p->getPositionY() + p->getSpriteHeight(); // Player's bottom edge
        float playerTopY = p->getPositionY();                          // Player's top edge
        float playerLeftX = p->getPositionX();                         // Player's left edge
        float playerRightX = p->getPositionX() + p->getSpriteWidth();  // Player's right edge

        float obstacleTopY = y;                                        // Obstacle's top edge
        float obstacleBottomY = y + getSpriteHeight();                 // Obstacle's bottom edge
        float obstacleLeftX = x;                                       // Obstacle's left edge
        float obstacleRightX = x + getSpriteWidth();                   // Obstacle's right edge

        // Check if the player is touching the obstacle
        bool isTouchingFromTop = (playerBottomY > obstacleTopY && playerBottomY < obstacleTopY + 10.0f) && // Close to the top
            (playerRightX > obstacleLeftX && playerLeftX < obstacleRightX);          // Horizontal overlap

        bool isTouchingFromBottom = (playerTopY < obstacleBottomY && playerTopY > obstacleBottomY - 10.0f) && // Close to the bottom
            (playerRightX > obstacleLeftX && playerLeftX < obstacleRightX);           // Horizontal overlap

        if (type == "spike") {
            // Spike-specific behavior: Damage the player if touched from the top
            if (isTouchingFromTop) {
                if (!hasDamagedPlayer) {
                    std::cout << "Lost an HP!" << std::endl;
                    // Add logic to reduce player HP here, e.g., p->reduceHP(1);
                    hasDamagedPlayer = true; // Mark as damaged to prevent multiple HP losses
                }
            }
            else {
                // Reset the flag when the player is no longer in contact
                hasDamagedPlayer = false;
            }
        }
        else if (type == "platform") {
            // Platform-specific behavior: Allow the player to stand on the platform
            if (isTouchingFromTop) {
                // Allow the player to stand on the platform
                p->setPosition(p->getPositionX(), obstacleTopY - p->getSpriteHeight());
                p->setVelocityY(0); // Stop the player from falling
                p->setOnGround(true); // Mark the player as on the ground
            }
            else if (isTouchingFromBottom) {
                // Ignore collisions from the bottom
                // No action needed
            }
        }
    }

    std::string type; // Type of obstacle (e.g., "spike", "platform")
    sf::Sprite sprite; // Sprite for rendering the obstacle
    sf::Texture texture; // Texture for the obstacle
    float x, y; // Position of the obstacle
    bool hasDamagedPlayer; // Flag to track if the player has been damaged
};
