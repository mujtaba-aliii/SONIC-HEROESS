#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"

class Knuckles : public PlayerCharacter {
public:
    // Constants for animation adjustments
    static constexpr int LEFT_RUN_CROP_X = 16;

    Knuckles(float sx, float sy)
        : PlayerCharacter(sx, sy)
    {
        // Define animation constants
        constexpr int FW = 64, FH = 64;
        constexpr float FT = 0.08f;
        constexpr int CROP_X = 8, CROP_W = 48;

        float adjustedY = sy - 36.0f;
        setPosition(sx, adjustedY);

        // Load right-facing animations
        animIdleR.load("Sprites/knuckles_idle.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animRunR.load("Sprites/knuckles_run.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpR.load("Sprites/knuckles_jump.png", FW, FH, 1, FT, CROP_X, CROP_W);
        //animHang.load("Sprites/knuckles_hang.png", FW, FH, 1, FT, CROP_X, CROP_W);

        // Load left-facing animations with specific crop values
        animIdleL.load("Sprites/knuckles_idleL.png", FW, FH, 1, FT, CROP_X, CROP_W);
        // Add a larger crop from the left for the running animation
        animRunL.load("Sprites/knuckles_runL.png", FW, FH, 8, FT, LEFT_RUN_CROP_X, CROP_W - (LEFT_RUN_CROP_X - CROP_X));
        animJumpL.load("Sprites/knuckles_jumpL.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animHangL = animHang;

        // Custom frame widths for running animations
        // Adjusted for Knuckles' sprite sheet based on the image provided
        int runRWidths[8] = { 48, 48, 48, 48, 48, 48, 48, 48 };
        int runLWidths[8] = { 48, 48, 48, 48, 48, 48, 48, 48 };

        // Apply custom frame widths
        animRunR.setCustomFrameWidths(runRWidths, 8);
        animRunL.setCustomFrameWidths(runLWidths, 8);

        // Set scale for all animations
        const float scale = 2.5f;

        // Apply scale to right-facing animations
        for (auto* a : { &animIdleR, &animRunR, &animJumpR, &animHang }) {
            a->getSprite().setScale(scale, scale);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }

        // Apply scale to left-facing animations
        for (auto* a : { &animIdleL, &animJumpL, &animHangL }) {
            a->getSprite().setScale(scale, scale);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }

        // Special handling for left running animation
        animRunL.getSprite().setScale(scale, scale);
        animRunL.getSprite().setOrigin(0, 0);
        // Adjust position to compensate for the crop
        animRunL.getSprite().setPosition(sx - LEFT_RUN_CROP_X * scale, adjustedY);

        current = &animIdleR;
    }

    void update(float dt, Level& level) override {
        // Keep track of the previous position
        float prevX = getPosition().x;

        // Apply standard movement physics
        applyMovement(dt, level);
        applyGravity(dt, level);

        // Get current velocity and ground state
        float vx = getVelocityX();
        bool onG = isOnGround();

        // Get current position
        sf::Vector2f pos = getPosition();

        // Determine which animation to use based on state
        Animation* next = nullptr;

        if (!onG) {
            // In air - use jump animation
            next = (vx >= 0 ? &animJumpR : &animJumpL);
        }
        else if (std::abs(vx) > 0.5f) {
            // Moving on ground - use run animation
            next = (vx > 0 ? &animRunR : &animRunL);
        }
        else {
            // Standing still - use idle animation
            // Preserve the last direction when stopping
            next = (current == &animIdleL || current == &animRunL || current == &animJumpL) ?
                &animIdleL : &animIdleR;
        }

        // Change animation if needed
        if (next != current) {
            if (current) current->stop();
            current = next;
            current->reset();
        }

        // Update animation
        current->play();
        current->update(dt);

        // Update sprite position - using the original vertical offset
        if (current == &animRunL) {
            // Apply special position adjustment for left running animation
            current->getSprite().setPosition(pos.x - LEFT_RUN_CROP_X * 2.5f, pos.y - 120.0f);
        }
        else {
            current->getSprite().setPosition(pos.x, pos.y - 120.0f);
        }
    }

    void render(sf::RenderWindow& window) override {
        // Only render the current animation sprite
        if (current) {
            window.draw(current->getSprite());
        }

        // Do NOT call base class render to avoid drawing debug boxes
        // PlayerCharacter::render(window);
    }

private:
    Animation animIdleR, animRunR, animJumpR, animHang;
    Animation animIdleL, animRunL, animJumpL, animHangL;
    Animation* current = nullptr;
};
