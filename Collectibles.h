#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class collectibles {
public:
    collectibles(float startX, float startY, const std::string& type)
        : x(startX), y(startY), type(type), rings(0)
    {
        const float scale = 4.0f; 

        if (type == "ring") {
            if (!texture.loadFromFile("Data/ring.png")) {
                std::cerr << "Failed to load texture for ring!" << std::endl;
            }
        }
        else if (type == "lives") {
            if (!texture.loadFromFile("Data/life.jpg")) {
                sprite.setScale(0.2f, 0.2f);
                std::cerr << "Failed to load texture for lives!" << std::endl;
            }
        }
        else if (type == "boost") {
            if (!texture.loadFromFile("Data/selector.png")) {
                std::cerr << "Failed to load texture for boost!" << std::endl;
            }
        }
        else {
            std::cerr << "Invalid collectible type: " << type << std::endl;
        }

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

    void updateDimensions() {
        if (sprite.getTexture()) {
            spriteWidth = sprite.getTexture()->getSize().x;
            spriteHeight = sprite.getTexture()->getSize().y;
        }
    }


    void setPosition(float nx, float ny) {
        x = nx;
        y = ny;
        sprite.setPosition(x, y);
    }

    void collectiblesCollisions(PlayerCharacter* p) {
        float distance = std::sqrt(std::pow(x - p->getPositionX(), 2.0f) + std::pow(y - p->getPositionY(), 2.0f));

        float threshold = 70.0f; 
        if (distance < threshold) {
            setPosition(-100, -100); 
            std::cout << "Collision detected for type: " << type << std::endl;

            if (type == "boost") {
                if (dynamic_cast<Sonic*>(p)) {
                    p->setMaxSpeed(p->getMaxSpeed() + 4.0f);
                    std::cout << "Sonic gained a speed boost! New max speed: " << p->getMaxSpeed() << std::endl;
                }
                else if (dynamic_cast<Tails*>(p)) {
                    Tails* tails = dynamic_cast<Tails*>(p);
                    if (tails) {
                        tails->extendFlightTime(4.0f); 
                        std::cout << "Tails gained +4 seconds of flight!" << std::endl;
                    }
                }
                else if (dynamic_cast<Knuckles*>(p)) {
                    Knuckles* knuckles = dynamic_cast<Knuckles*>(p);
                    if (knuckles) {
                        knuckles->activateInvincibility(15.0f); 
                        std::cout << "Knuckles is now invincible for 15 seconds!" << std::endl;
                    }
                }
            }
            else if (type == "rings") {
                BackgroundMusic.setVolume(100);
                BackgroundMusic.play();
                increaserings();
            }
			else if (type == "lives") {
				p->setHp(p->getHP() + 1);
			}
			else {
				std::cout << "Invalid collectible type: " << type << std::endl;
			}
        }
    }

    void increaserings() {
        rings++;
    }
    int getrings() {
        return rings;
    }
protected:
    std::string type; // Type of collectible (e.g., "ring", "lives", "boost")
    sf::Sprite sprite;
    sf::Texture texture;
    int spriteWidth, spriteHeight;
    float x, y;
    int rings;
    sf::Music BackgroundMusic;
    sf::Texture backgroundMusicTexture;

};

