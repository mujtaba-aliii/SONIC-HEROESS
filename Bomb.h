#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
struct Bullet {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float gravity;
    bool active;
    sf::Sprite sprite;

    Bullet() : gravity(300.f), active(false) {}

    void init(sf::Vector2f pos, sf::Vector2f vel, const sf::Texture& texture) {
        position = pos;
        velocity = vel;
        sprite.setTexture(texture);
        sprite.setScale(2.5f, 2.5f);
        sprite.setPosition(pos);
        active = true;
    }

    void update(float dt) {
        if (!active) return;
        velocity.y += gravity * dt;
        position += velocity * dt;
        sprite.setPosition(position);
    }

    void render(sf::RenderWindow& window) {
        if (active)
            window.draw(sprite);
    }

};
