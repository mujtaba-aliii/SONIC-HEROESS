#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Animation {
public:
    static constexpr int MAX_FRAMES = 16;

    Animation()
        : frameCount(0)
        , currentFrame(0)
        , elapsed(0.f)
        , frameTime(0.1f)
        , loop(true)
        , paused(false)
    {
    }

   
    void load(const std::string& filename,
        int frameW, int frameH,
        int count, float ft,
        int cropOffsetX,
        int croppedW)
    {
        sf::Image img;
        if (!img.loadFromFile(filename)) {
            std::cerr << "Animation::load: failed to load " << filename << std::endl;
            return;
        }

        img.createMaskFromColor(sf::Color(0, 0, 0),0);

        if (!texture.loadFromImage(img)) {
            std::cerr << "Animation::load: failed to create texture from image" << std::endl;
            return;
        }

        sprite.setTexture(texture);

        originalFrameW = frameW;
        originalFrameH = frameH;
        frameCount = count;
        frameTime = ft;
        cropX = cropOffsetX;
        drawW = (croppedW > 0 ? croppedW : (frameW - 2 * cropOffsetX));

        for (int i = 0; i < frameCount && i < MAX_FRAMES; i++) {
            int left = i * originalFrameW + cropX;
            frames[i] = sf::IntRect(left, 0, drawW, originalFrameH);
        }

        currentFrame = 0;
        sprite.setTextureRect(frames[0]);
    }

    void update(float dt) {
        if (paused || frameCount <= 1) return;

        elapsed += dt;
        while (elapsed >= frameTime) {
            currentFrame = (currentFrame + 1) % frameCount;
            if (!loop && currentFrame == 0) {
                currentFrame = frameCount - 1;  
                paused = true;
                break;
            }

            sprite.setTextureRect(frames[currentFrame]);
            elapsed -= frameTime;
        }
    }


    void play() {
        paused = false;
    }

    void stop() {
        paused = true;
        currentFrame = 0;
        if (frameCount > 0) {
            sprite.setTextureRect(frames[0]);
        }
    }

    void reset() {
        currentFrame = 0;
        elapsed = 0.f;
        paused = false;
        if (frameCount > 0) {
            sprite.setTextureRect(frames[0]);
        }
    }
    void setCustomFrameWidths(const int* widths, int num_frames) {
        if (num_frames > frameCount || num_frames > MAX_FRAMES) {
            num_frames = std::min(frameCount, MAX_FRAMES);
        }

        for (int i = 0; i < frameCount && i < MAX_FRAMES; i++) {
            int left = 0;
            for (int j = 0; j < i; j++) {
                left += (j < num_frames) ? widths[j] : originalFrameW;
            }

            int width = (i < num_frames) ? widths[i] : drawW;

            frames[i] = sf::IntRect(left, 0, width, originalFrameH);
        }

        if (frameCount > 0) {
            sprite.setTextureRect(frames[currentFrame]);
        }
    }
    sf::Sprite& getSprite() { return sprite; }

private:
    sf::Texture  texture;
    sf::Sprite   sprite;
    sf::IntRect  frames[MAX_FRAMES];
    int          frameCount;
    int          currentFrame;
    float        elapsed;
    float        frameTime;
    bool         loop;
    bool         paused;
    int          originalFrameW, originalFrameH;
    int          cropX, drawW;
};