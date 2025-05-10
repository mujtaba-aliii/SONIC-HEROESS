#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"

class Sonic : public PlayerCharacter {
public:
    Sonic(float sx, float sy)
        : PlayerCharacter(sx, sy)
    {
        // Define animation constants
        constexpr int FW = 64, FH = 64;
        constexpr float FT = 0.08f;
        constexpr int CROP_X = 0, CROP_W = 32;

        float adjustedY = sy - 36.0f;
        setPosition(sx, adjustedY);

        // Load animations
        animIdleR.load("Data/0right_still.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animRunR.load("Data/0right_run.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpR.load("Data/sonic_jumpR.png", FW, FH, 1, FT, CROP_X, 36);
        animHang.load("Data/sonic_hang.png", FW, FH, 1, FT, CROP_X, CROP_W);

        // Load left-facing animations
        animIdleL.load("Data/0left_still.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animRunL.load("Data/0left_run.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpL.load("Data/sonic_jumpL.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animHangL = animHang;

        // Custom frame widths for running animations
        // These values need to be measured from your specific sprite sheets
        int runRWidths[8] = { 40, 40, 40, 40, 40, 40, 40, 40 };
        int runLWidths[8] = { 40, 40, 40, 40, 40, 40, 40, 40 };

        // Apply custom frame widths if your Animation class has the method
        // If you haven't added the setCustomFrameWidths method to your Animation class,
        // you'll need to add it first or comment these lines out
        animRunR.setCustomFrameWidths(runRWidths, 8);
        animRunL.setCustomFrameWidths(runLWidths, 8);

        // Set scale for all animations
        const float scale = 2.5f;

        // Apply scale to all animations
        for (auto* a : { &animIdleR, &animIdleL, &animRunR, &animRunL,
                       &animJumpR, &animJumpL, &animHang, &animHangL }) {
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

        // Update sprite position
        current->getSprite().setPosition(pos.x, pos.y - 98.0f);
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