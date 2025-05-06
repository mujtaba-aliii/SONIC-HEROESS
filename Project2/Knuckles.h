// File: Knuckles.h
#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"
#include "Level.h"
#include <cmath>

class Knuckles : public PlayerCharacter {
public:
    Knuckles(float sx, float sy)
        : PlayerCharacter(sx, sy)
    {
        // Adjust Y so Knuckles sits on the ground correctly
        float adjustedY = sy - 36.f;
        setPosition(sx, adjustedY);

        // Frame dimensions & timings
        constexpr int IDLE_FW = 491, IDLE_FH = 47;
        constexpr int RUN_FW = 388, RUN_FH = 48;
        constexpr int JUMP_FW = 289, JUMP_FH = 43;
        constexpr int HANG_FW = 391, HANG_FH = 46;
        constexpr float FT_IDLE = 0.10f;
        constexpr float FT_RUN = 0.08f;
        constexpr float FT_JUMP = 0.12f;
        constexpr int CROP_X = 0;

        // Load right-facing animations
        animIdleR.load("Sprites/knuckles_idle.png", IDLE_FW, IDLE_FH, 10, FT_IDLE, CROP_X, IDLE_FW / 10);
        animRunR.load("Sprites/knuckles_run.png", RUN_FW, RUN_FH, 8, FT_RUN, CROP_X, RUN_FW / 8);
        animJumpR.load("Sprites/knuckles_jump.png", JUMP_FW, JUMP_FH, 6, FT_JUMP, CROP_X, JUMP_FW / 6);
        animHangR.load("Sprites/knuckles_hang.png", HANG_FW, HANG_FH, 7, FT_RUN, CROP_X, HANG_FW / 7);

        // Load left-facing animations
        animIdleL.load("Sprites/knuckles_idleL.png", IDLE_FW, IDLE_FH, 10, FT_IDLE, CROP_X, IDLE_FW / 10);
        animRunL.load("Sprites/knuckles_runL.png", RUN_FW, RUN_FH, 8, FT_RUN, CROP_X, RUN_FW / 8);
        animJumpL.load("Sprites/knuckles_jumpL.png", JUMP_FW, JUMP_FH, 6, FT_JUMP, CROP_X, JUMP_FW / 6);
        animHangL.load("Sprites/knuckles_hangL.png", HANG_FW, HANG_FH, 7, FT_RUN, CROP_X, HANG_FW / 7);

        // Custom frame widths
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

        // Scale & position all sprites
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

    // Standard update logic
    void update(float dt, Level& level, int windowWidth, int windowHeight) override {
        applyMovement(dt, level);
        applyGravity(dt, level);

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

        // Offset so sprite feet align properly
        current->getSprite().setPosition(pos.x, pos.y - 120.f);
        sprite.setPosition(pos);

        clampPosition(windowWidth, windowHeight,
            level.getWidth() * level.getCellSize());
    }

    // Draw Knuckles
    void render(sf::RenderWindow& window) override {
        if (current) window.draw(current->getSprite());
    }

    // Force Knuckles into hang animation
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



    // Follower behavior
    void followTarget(float tx, float ty, float vX, float dt, Level& level) override {
        PlayerCharacter::followTarget(tx, ty, vX, dt, level);

        // Match animation to movement
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

private:
    Animation animIdleR, animRunR, animJumpR, animHangR;
    Animation animIdleL, animRunL, animJumpL, animHangL;
    Animation* current = nullptr;
};
