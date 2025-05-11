#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"
#include "Level.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

class Tails : public PlayerCharacter {
public:
    Tails(float sx, float sy,
        float gravity, float terminalVel,
        float maxSpeed, float acceleration,
        float deceleration)
        : PlayerCharacter(gravity,
            terminalVel,
            maxSpeed,
            acceleration,
            deceleration)
    {
        jumpStrength = -20.f;
        float adjustedY = sy - 36.f;
        setPosition(sx, adjustedY);

        constexpr int IDLE_FW = 491, IDLE_FH = 50;
        constexpr int RUN_FW = 391, RUN_FH = 49;
        constexpr int JUMP_FW = 292, JUMP_FH = 48;
        constexpr float FT = 0.08f;
        constexpr int   CROP_X = 0;

        animIdleR.load("Sprites/tails_idle.png", IDLE_FW, IDLE_FH, 10, FT, CROP_X, IDLE_FW / 10);
        animIdleL.load("Sprites/tails_idleL.png", IDLE_FW, IDLE_FH, 10, FT, CROP_X, IDLE_FW / 10);
        animRunR.load("Sprites/tails_run.png", RUN_FW, RUN_FH, 8, FT, CROP_X, RUN_FW / 8);
        animRunL.load("Sprites/tails_runL.png", RUN_FW, RUN_FH, 8, FT, CROP_X, RUN_FW / 8);
        animJumpR.load("Sprites/tails_jump.png", JUMP_FW, JUMP_FH, 6, FT, CROP_X, JUMP_FW / 6);
        animJumpL.load("Sprites/tails_jumpL.png", JUMP_FW, JUMP_FH, 6, FT, CROP_X, JUMP_FW / 6);

        constexpr int GLIDE_FW = 217, GLIDE_FH = 45;
        constexpr int GLIDE_FRAMES = 4;
        animGlideR.load("Sprites/tails_fly.png",
            GLIDE_FW, GLIDE_FH,
            GLIDE_FRAMES, FT, CROP_X, GLIDE_FW / GLIDE_FRAMES);
        animGlideL.load("Sprites/tails_flyL.png",
            GLIDE_FW, GLIDE_FH,
            GLIDE_FRAMES, FT, CROP_X, GLIDE_FW / GLIDE_FRAMES);

        int idleW[10] = { 49,49,49,49,49,49,49,49,49,49 };
        int runW[8] = { 49,49,49,49,49,49,49,49 };
        int jumpW[6] = { 48,48,48,48,48,48 };
        int glideW[4] = { 54,54,54,55 };

        animIdleR.setCustomFrameWidths(idleW, 10);
        animIdleL.setCustomFrameWidths(idleW, 10);
        animRunR.setCustomFrameWidths(runW, 8);
        animRunL.setCustomFrameWidths(runW, 8);
        animJumpR.setCustomFrameWidths(jumpW, 6);
        animJumpL.setCustomFrameWidths(jumpW, 6);
        animGlideR.setCustomFrameWidths(glideW, 4);
        animGlideL.setCustomFrameWidths(glideW, 4);

        constexpr float scale = 2.5f;
        for (auto* a : { &animIdleR, &animRunR, &animJumpR,
                         &animIdleL, &animRunL, &animJumpL,
                         &animGlideR, &animGlideL })
        {
            a->getSprite().setScale(scale, scale);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }
        current = &animIdleR;
    }

    void handleInput(const sf::Event& ev) override {
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Up) {
            if (onGround) {
                gliding = false;
                glideExpired = false;
                glideTimerStarted = false;
                glidePhaseActive = false;
                PlayerCharacter::handleInput(ev);
            }
            else if (!gliding && !glideExpired) {
                gliding = true;
                if (!glideTimerStarted) {
                    glideClock.restart();
                    glideTimerStarted = true;
                }
                glidePhaseActive = true;
                velocityY = glideInitialVelY;
            }
        }
        else if (ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::Up) {
            if (gliding) {
                gliding = false;
            }
            PlayerCharacter::handleInput(ev);
        }
        else {
            PlayerCharacter::handleInput(ev);
        }
    }

    void update(float dt, Level& level, int windowWidth, int windowHeight) override {
        if (gliding && glideClock.getElapsedTime().asSeconds() >= currentGlideTime) {
            gliding = false;
            glideExpired = true;
        }

        applyMovement(dt, level);

        float gFactor = 1.f;
        if (gliding)                       gFactor = glideGravityFactor;
        else if (velocityY > 0.f && !glideExpired)
            gFactor = postGlideGravityFactor;

        velocityY = std::min(velocityY + gravity * gFactor * dt * 60.f,
            terminalVel);

        float newY = y + velocityY * dt * 60.f;
        bool hitL = level.collidesAt(x + 8, newY + spriteHeight),
            hitR = level.collidesAt(x + spriteWidth - 8, newY + spriteHeight);

        if (hitL || hitR) {
            onGround = true;
            int cell = level.getCellSize();
            int blockY = (int(newY + spriteHeight) / cell) * cell;
            y = blockY - spriteHeight;
            velocityY = 0;

            gliding = false;
            glideExpired = false;
            glideTimerStarted = false;
            glidePhaseActive = false;
        }
        else {
            y = newY;
            onGround = false;
        }

        pickAndAdvanceAnimation(dt);
        clampPosition(windowWidth, windowHeight,
            level.getWidth() * level.getCellSize());
    }

    void extendFlightTime(float seconds) {
        currentGlideTime += seconds;
        std::cout << "Tails' flight time extended by " << seconds << " seconds! New glide time: " << currentGlideTime << " seconds." << std::endl;
    }

    void followTarget(float tx, float ty, float vX, float dt, Level& level) override {
        PlayerCharacter::followTarget(tx, ty, vX, dt, level);
        pickAndAdvanceAnimation(dt);
    }

    void render(sf::RenderWindow& window) override {
        if (current) window.draw(current->getSprite());
    }

    bool isGliding() const { return gliding; }
    bool isGlidePhaseActive() const { return glidePhaseActive; }

private:
    void pickAndAdvanceAnimation(float dt) {
        float vx = getVelocityX();
        bool onG = isOnGround();
        sf::Vector2f pos = getPosition();
        Animation* next = nullptr;

        if (glidePhaseActive) {
            next = (vx >= 0 ? &animGlideR : &animGlideL);
        }
        else if (!onG) {
            next = (vx >= 0 ? &animJumpR : &animJumpL);
        }
        else if (std::abs(vx) > 0.5f) {
            next = (vx > 0 ? &animRunR : &animRunL);
        }
        else {
            next = ((current == &animIdleL || current == &animRunL || current == &animJumpL)
                ? &animIdleL : &animIdleR);
        }

        if (next != current) {
            current->stop();
            current = next;
            current->reset();
        }
        current->play();
        current->update(dt);

        current->getSprite().setPosition(pos.x, pos.y - 120.f);
        sprite.setPosition(pos);
    }

    bool      gliding = false;
    bool      glideExpired = false;
    bool      glideTimerStarted = false;
    bool      glidePhaseActive = false;
    sf::Clock glideClock;

    float currentGlideTime = maxGlideTime; // Dynamic glide time
    static constexpr float maxGlideTime = 7.f;
    static constexpr float glideGravityFactor = 0.3f;
    static constexpr float postGlideGravityFactor = 0.2f;
    static constexpr float glideInitialVelY = -8.f;

    Animation animIdleR, animRunR, animJumpR, animGlideR;
    Animation animIdleL, animRunL, animJumpL, animGlideL;
    Animation* current = nullptr;
};
