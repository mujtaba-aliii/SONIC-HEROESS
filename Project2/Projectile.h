//#pragma once
//#include<iostream>
//
//class Projectile {
//public:
//	Projectile(float startx, float starty) {
//		x = startx;
//		y = starty;
//	}
//	float getPositionX() { return x; }
//	float getPositionY() { return y; }
//	float setPosition(float startX, float startY) { x = startX;y = startY; }
//	void update() {}
//protected:
//	float x, y;
//	float speed = 2.0f;
//};
#pragma once
#include <iostream>
#include <cmath>

class Projectile {
public:
    Projectile(float startx, float starty) {
        x = startx;
        y = starty;
        vx = 0;
        vy = 0;
        if (!shootTexture.loadFromFile("Data/shoot.png")) {
            std::cout << "Failed to load shoot.png" << std::endl;
        }
        shootSprite.setTexture(shootTexture);
        shootSprite.setScale(0.5f, 0.5f); // Adjust scale if needed

    }

    float getPositionX() { return x; }
    float getPositionY() { return y; }

    void setPosition(float startX, float startY) {
        x = startX;
        y = startY;
    }

    void setDirectionToward(float targetX, float targetY) {
        float dx = targetX - x;
        float dy = targetY - y;
        float length = std::sqrt(dx * dx + dy * dy);
        if (length != 0) {
            vx = speed * (dx / length);
            vy = speed * (dy / length);
        }
    }
    void render(sf::RenderWindow& window) {
        window.draw(shootSprite);
    }
    void update() {
        x += vx;
        y += vy;
        shootSprite.setPosition(x, y);
    }

protected:
    float x, y;
    float vx, vy;       // velocity in x and y
    float speed = 2.0f; // change as needed
    sf::Texture shootTexture;
    sf::Sprite shootSprite;

};
