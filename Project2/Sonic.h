// File: Sonic.h
#pragma once

#include "PlayerCharacter.h"
#include "Animation.h"
#include "Level.h"
#include <cmath>

class Sonic : public PlayerCharacter {
public:
    Sonic(float sx, float sy)
        : PlayerCharacter(sx, sy)
    {
        constexpr int FW = 320, FH = 40;
        constexpr float FT = 0.08f;
        constexpr int CROP_X = 0, CROP_W = 320;

        float adjustedY = sy - 36.f;
        setPosition(sx, adjustedY);

        animIdleR.load("Data/sonic_idleR.png", 329, 42, 7, FT, CROP_X, 329 / 7);
        animRunR.load("Data/0right_run.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpR.load("Data/sonic_jumpR.png", FW, FH, 1, FT, 0, 36);
        animHang.load("Data/sonic_hang.png", 392, 44, 8, FT, CROP_X, 392 / 8);
        animIdleL.load("Data/sonic_idleL.png", FW, FH, 1, FT, CROP_X, CROP_W);
        animRunL.load("Data/0left_run.png", FW, FH, 8, FT, CROP_X, CROP_W);
        animJumpL.load("Data/sonic_jumpL.png", FW, FH, 1, FT, 0, 36);
        animHangL.load("Data/sonic_hangL.png", 392, 44, 8, FT, CROP_X, 392 / 8);

        int idleW[7] = { 47,47,47,47,47,47,47 };
        animIdleR.setCustomFrameWidths(idleW, 7);
        animIdleL.setCustomFrameWidths(idleW, 7);

        int rR[8] = { 40,40,40,40,40,40,40,40 };
        int rL[8] = { 40,40,40,40,40,40,40,40 };
        animRunR.setCustomFrameWidths(rR, 8);
        animRunL.setCustomFrameWidths(rL, 8);

        int hR[8] = { 49,49,49,49,49,49,49,49 };
        animHang.setCustomFrameWidths(hR, 8);
        animHangL.setCustomFrameWidths(hR, 8);

        constexpr float s = 2.5f;
        for (auto* a : { &animIdleR, &animRunR, &animJumpR, &animHang,
                         &animIdleL, &animRunL, &animJumpL, &animHangL })
        {
            a->getSprite().setScale(s, s);
            a->getSprite().setOrigin(0, 0);
            a->getSprite().setPosition(sx, adjustedY);
        }
        current = &animIdleR;
    }

    void update(float dt, Level& level, int windowWidth, int windowHeight) override {
        applyMovement(dt, level);
        applyGravity(dt, level);

        float vx = getVelocityX();
        bool onG = isOnGround();
        sf::Vector2f pos = getPosition();
        Animation* next = nullptr;

        if (!onG)           next = (vx >= 0 ? &animJumpR : &animJumpL);
        else if (std::abs(vx) > 0.5f) next = (vx > 0 ? &animRunR : &animRunL);
        else next = ((current == &animIdleL || current == &animRunL || current == &animJumpL)
            ? &animIdleL : &animIdleR);

        if (next != current) { current->stop(); current = next; current->reset(); }
        current->play(); current->update(dt);

        current->getSprite().setPosition(pos.x, pos.y - 100.0f);
        sprite.setPosition(pos);
        clampPosition(windowWidth, windowHeight, level.getWidth() * level.getCellSize());
    }

    void render(sf::RenderWindow& window) override {
        if (current) window.draw(current->getSprite());
    }

    void followTarget(float tx, float ty, float vX, float dt, Level& level) override {
        PlayerCharacter::followTarget(tx, ty, vX, dt, level);

        float vx = vX;
        bool onG = isOnGround();
        sf::Vector2f pos = getPosition();
        Animation* next = nullptr;

        if (!onG)           next = (vx >= 0 ? &animJumpR : &animJumpL);
        else if (std::abs(vx) > 0.5f) next = (vx > 0 ? &animRunR : &animRunL);
        else next = ((current == &animIdleL || current == &animRunL || current == &animJumpL)
            ? &animIdleL : &animIdleR);

        if (next != current) { current->stop(); current = next; current->reset(); }
        current->play(); current->update(dt);
        current->getSprite().setPosition(pos.x, pos.y - 98.0f);
    }

    // ─── Force Hang Animation ───────────────────────────────────
    void forceHang(bool facingRight) {
        current->stop();
        Animation* hangAnim = (facingRight ? &animHang : &animHangL);
        current = hangAnim;
        current->reset();
        current->play();
        auto pos = getPosition();
        current->getSprite().setPosition(pos.x, pos.y);
    }

private:
    Animation animIdleR, animRunR, animJumpR, animHang;
    Animation animIdleL, animRunL, animJumpL, animHangL;
    Animation* current = nullptr;
};
