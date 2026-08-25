// File: Knuckles.h
#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"
#include "Level.h"
#include <cmath>

class Knuckles : public PlayerCharacter {
public:
    Knuckles(float sx, float sy,
        float gravity, float terminalVel,
        float maxSpeed, float acceleration,
        float deceleration)
        : PlayerCharacter(gravity,
            terminalVel,
            maxSpeed,
            acceleration,
            deceleration)
    {

        float adjustedY = sy - 36.f;
        setPosition(sx, adjustedY);

        constexpr int IDLE_FW = 491, IDLE_FH = 47;
        constexpr int RUN_FW = 388, RUN_FH = 48;
        constexpr int JUMP_FW = 289, JUMP_FH = 43;
        constexpr int HANG_FW = 391, HANG_FH = 46;
        constexpr float FT = 0.10f;
        constexpr int CROP_X = 0;

        animIdleR.load("Sprites/knuckles_idle.png", IDLE_FW, IDLE_FH, 10, FT, CROP_X, IDLE_FW / 10);
        animRunR.load("Sprites/knuckles_run.png", RUN_FW, RUN_FH, 8, FT, CROP_X, RUN_FW / 8);
        animJumpR.load("Sprites/knuckles_jump.png", JUMP_FW, JUMP_FH, 6, FT, CROP_X, JUMP_FW / 6);
        animHangR.load("Sprites/knuckles_hang.png", HANG_FW, HANG_FH, 7, FT, CROP_X, HANG_FW / 7);
		animBreakR.load("Sprites/knuckles_breakR.png", 484, 46, 10, 0.1f, 0, 484 / 10);
        
        animIdleL.load("Sprites/knuckles_idleL.png", IDLE_FW, IDLE_FH, 10, FT, CROP_X, IDLE_FW / 10);
        animRunL.load("Sprites/knuckles_runL.png", RUN_FW, RUN_FH, 8, FT, CROP_X, RUN_FW / 8);
        animJumpL.load("Sprites/knuckles_jumpL.png", JUMP_FW, JUMP_FH, 6, FT, CROP_X, JUMP_FW / 6);
        animHangL.load("Sprites/knuckles_hangL.png", HANG_FW, HANG_FH, 7, FT, CROP_X, HANG_FW / 7);
        animBreakL.load("Sprites/knuckles_breakL.png", 484, 46, 10, 0.1f, 0, 484 / 10);
        
        int idleW[10] = { 49,49,49,49,49,49,49,49,49,49 };
        animIdleR.setCustomFrameWidths(idleW, 10);
        animIdleL.setCustomFrameWidths(idleW, 10);

        int runW[8] = { 48,48,48,48,48,48,48,48 };
        animRunR.setCustomFrameWidths(runW, 8);
        animRunL.setCustomFrameWidths(runW, 8);

        int jumpW[6] = { 48,48,48,48,48,48 };
        animJumpR.setCustomFrameWidths(jumpW, 6);
        animJumpL.setCustomFrameWidths(jumpW, 6);

        int hangW[7] = { 55,55,55,55,55,55,55 };
        animHangR.setCustomFrameWidths(hangW, 7);
        animHangL.setCustomFrameWidths(hangW, 7);

        
        constexpr float SCALE = 2.5f;
        for (auto* a : { &animIdleR, &animRunR, &animJumpR, &animHangR,
                         &animIdleL, &animRunL, &animJumpL, &animHangL })
        {
            a->getSprite().setScale(SCALE, SCALE);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }

        current = &animIdleR;
    }

    void update(float dt, Level& level, int windowWidth, int windowHeight) override {
        applyMovement(dt, level);
        applyGravity(dt, level);
		breakWall(level, movingRight);
        float vx = getVelocityX();
        bool onG = isOnGround();
        sf::Vector2f pos = getPosition();
        Animation* next = nullptr;

        if (!onG) {
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

        clampPosition(windowWidth, windowHeight,
            level.getWidth() * level.getCellSize());
    }

    void render(sf::RenderWindow& window) override {
        if (current) window.draw(current->getSprite());
    }

    void forceHang(bool facingRight) {
        if (current) current->stop();
        Animation* hangAnim = facingRight ? &animHangR : &animHangL;
        current = hangAnim;
        current->reset();
        current->play();
        auto pos = getPosition();
        current->getSprite().setPosition(pos.x, pos.y);
        sprite.setPosition(pos);
    }


    void followTarget(float tx, float ty, float vX, float dt, Level& level) override {
        PlayerCharacter::followTarget(tx, ty, vX, dt, level);


        float vx = vX;
        bool onG = isOnGround();
        sf::Vector2f pos = getPosition();
        Animation* next = nullptr;
        if (!onG) {
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
    }



    void applyMovement(float dt, Level& level) {
        if (movingLeft) {
            velocityX = std::max(velocityX - acceleration * dt * 60.f, -maxSpeed);
        }
        else if (movingRight) {
            velocityX = std::min(velocityX + acceleration * dt * 60.f, maxSpeed);
        }
        else {
            if (velocityX > 0) velocityX = std::max(0.f, velocityX - deceleration * dt * 60.f * 3);
            else              velocityX = std::min(0.f, velocityX + deceleration * dt * 60.f * 3);
        }

        float newX = x + velocityX * dt * 60.f;

        bool collL = level.collidesAt(newX + 8, y + spriteHeight - 2, true);
        bool collR = level.collidesAt(newX + spriteWidth - 8, y + spriteHeight - 2, true);

        if (!collL && !collR) {
            x = newX;
        }
        else {
            velocityX = 0;
        }
    }

    void activateInvincibility(float seconds) {
        invincible = true; 
        invincibilityClock.restart(); 
        invincibilityDuration = seconds; 
        std::cout << "Knuckles is invincible for " << seconds << " seconds!" << std::endl;
    }

    void breakWall(Level& level, bool facingRight) {
        float checkX = facingRight ? x + spriteWidth : x - level.getCellSize();
        float checkY = y + spriteHeight - 1; 
        char cell = level.getGridCell(checkX, checkY);
        if (cell == 'k') {
            Animation* breakAnim = facingRight ? &animBreakR : &animBreakL;
            if (current) current->stop();
            current = breakAnim;
            current->reset();
            current->play();
			auto pos = getPosition();
            current->getSprite().setPosition(pos);
            level.setGridCell(checkX, checkY, ' ');
        }
    }






private:
    Animation animIdleR, animRunR, animJumpR, animHangR, animBreakR;
    Animation animIdleL, animRunL, animJumpL, animHangL, animBreakL;
    Animation* current = nullptr;
	bool invincible = false; // Flag to indicate if Knuckles is invincible
	sf::Clock invincibilityClock; // Clock to track invincibility duration
	float invincibilityDuration = 0.f; // Duration of invincibility
};
