#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"

class Tails : public PlayerCharacter {
public:
    Tails(float sx, float sy)
        : PlayerCharacter(sx, sy)
    {
        // Define animation constants
        constexpr int FW = 64, FH = 64;
        constexpr float FT = 0.08f;
        constexpr int CROP_X = 8, CROP_W = 48;

        float adjustedY = sy - 36.0f;
        setPosition(sx, adjustedY);

        // Load animations - using paths from original Tails code
        animIdleR.load("Sprites/tails_idle.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animRunR.load("Sprites/tails_run.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpR.load("Sprites/tails_jump.png", FW, FH, 1, FT, CROP_X, CROP_W);
        //animHang.load("Sprites/tails_hang.png", FW, FH, 1, FT, CROP_X, CROP_W);

        // Load left-facing animations
        animIdleL.load("Sprites/tails_idleL.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animRunL.load("Sprites/tails_runL.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpL.load("Sprites/tails_jumpL.png", FW, FH, 1, FT, CROP_X, 16);
        animHangL = animHang;

        // Custom frame widths for running animations
        // Adjusted for Tails' sprite sheet based on the image provided
        int runRWidths[8] = { 48, 48, 48, 48, 48, 48, 48, 48 };
        int runLWidths[8] = { 48, 48, 48, 48, 48, 48, 48, 48 };

        // Apply custom frame widths if your Animation class has the method
        animRunR.setCustomFrameWidths(runRWidths, 8);
        animRunL.setCustomFrameWidths(runLWidths, 8);

        // Set scale for all animations
        const float scale = 2.5f;

        // Apply scale to all animations
        for (auto* a : { &animIdleR, &animRunR, &animJumpR, &animHang }) {
            a->getSprite().setScale(scale, scale);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }

        // Apply scale to left-facing animations
        for (auto* a : { &animIdleL, &animRunL, &animJumpL, &animHangL }) {
            a->getSprite().setScale(scale, scale);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }

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

        // Update sprite position - using the original Tails offset
        current->getSprite().setPosition(pos.x, pos.y - 120.0f);
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
