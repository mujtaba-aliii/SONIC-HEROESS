#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "PlayerCharacter.h" 

class obstacles {
public:
    obstacles(float startX, float startY, const std::string& type, int cellSize)
        : x(startX), y(startY), type(type), hasDamagedPlayer(false)
    {

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

        sprite.setTexture(texture);


        sf::Vector2u textureSize = texture.getSize();
        float scaleX = float(cellSize) / textureSize.x;
        float scaleY = float(cellSize) / textureSize.y;
        sprite.setScale(scaleX, scaleY);

        sprite.setPosition(x, y);
    }


    int getSpriteWidth() const {
        return sprite.getTexture() ? sprite.getTexture()->getSize().x : 0;
    }


    int getSpriteHeight() const {
        return sprite.getTexture() ? sprite.getTexture()->getSize().y : 0;
    }


    void render(sf::RenderWindow& window) {
        window.draw(sprite);
    }


    void setScale(float sx, float sy) {
        sprite.setScale(sx, sy);
    }


    void update(float dt, PlayerCharacter* p) {
        handleCollision(p);
    }


    void setPosition(float nx, float ny) {
        x = nx;
        y = ny;
        sprite.setPosition(x, y);
    }

protected:

    void handleCollision(PlayerCharacter* p) {

        float playerBottomY = p->getPositionY() + p->getSpriteHeight();
        float playerTopY = p->getPositionY();
        float playerLeftX = p->getPositionX();
        float playerRightX = p->getPositionX() + p->getSpriteWidth();

        float obstacleTopY = y;
        float obstacleBottomY = y + getSpriteHeight();
        float obstacleLeftX = x;
        float obstacleRightX = x + getSpriteWidth();

        bool isTouchingFromTop = (playerBottomY > obstacleTopY && playerBottomY < obstacleTopY + 30.0f) &&
            (playerRightX > obstacleLeftX && playerLeftX < obstacleRightX);

        bool isTouchingFromBottom = (playerTopY < obstacleBottomY && playerTopY > obstacleBottomY - 10.0f) &&
            (playerRightX > obstacleLeftX && playerLeftX < obstacleRightX);

        if (type == "spike") {
            if (isTouchingFromTop) {
                if (!hasDamagedPlayer) {
                    std::cout << "lost an hp" << std::endl;
                    p->decreaseLives(0);
                    hasDamagedPlayer = true;
                }
            }
            else {
                hasDamagedPlayer = false;
            }
        }
        else if (type == "platform") {

            if (isTouchingFromTop) {

                p->setPosition(p->getPositionX(), obstacleTopY - p->getSpriteHeight());
                p->setVelocityY(0);
                p->setOnGround(true);
            }
        }
    }

    std::string type;
    sf::Sprite sprite;
    sf::Texture texture;
    float x, y;
    bool hasDamagedPlayer;
};
