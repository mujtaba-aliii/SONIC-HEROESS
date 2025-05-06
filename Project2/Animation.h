#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Animation {
public:
    static constexpr int MAX_FRAMES = 16;  // adjust as needed

    Animation()
        : frameCount(0)
        , currentFrame(0)
        , elapsed(0.f)
        , frameTime(0.1f)
        , loop(true)
        , paused(false)
    {
    }

    // filename: sprite‑sheet with frames laid out horizontally
    // fw, fh: size of each frame
    // fc:    how many frames in the sheet (<= MAX_FRAMES)
    // ft:    seconds per frame
    // lp:    should it loop?
    void load(const std::string& atlasPath,
        int frameW, int frameH,
        int count, float ft,
        int cropOffsetX,
        int croppedW)
    {
        // 1) Load into an sf::Image so we can mask out any stray black pixels:
        sf::Image img;
        if (!img.loadFromFile(atlasPath)) {
            std::cerr << "Animation::load: failed to load " << atlasPath << std::endl;
            return;
        }

        // 2) Treat pure black as transparent
        img.createMaskFromColor(sf::Color(0, 0, 0), /*alpha=*/0);

        // 3) Upload the cleaned image into the texture
        if (!texture.loadFromImage(img)) {
            std::cerr << "Animation::load: failed to create texture from image" << std::endl;
            return;
        }

        sprite.setTexture(texture);

        // 4) Set up frame data
        originalFrameW = frameW;
        originalFrameH = frameH;
        frameCount = count;
        frameTime = ft;
        cropX = cropOffsetX;
        drawW = (croppedW > 0 ? croppedW : (frameW - 2 * cropOffsetX));

        // Pre-calculate all frame rectangles
        for (int i = 0; i < frameCount && i < MAX_FRAMES; i++) {
            int left = i * originalFrameW + cropX;
            frames[i] = sf::IntRect(left, 0, drawW, originalFrameH);
        }

        // Initialize on frame zero
        currentFrame = 0;
        sprite.setTextureRect(frames[0]);
    }

    void update(float dt) {
        if (paused || frameCount <= 1) return;

        elapsed += dt;
        if (elapsed >= frameTime) {
            elapsed -= frameTime;
            currentFrame = (currentFrame + 1) % frameCount; // Loop through frames
            sprite.setTextureRect(frames[currentFrame]);   // Set the texture rectangle for the current frame
        }
    }


    void play() {
        paused = false;
    }

    void stop() {
        paused = true;
        // Reset to first frame when stopped
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
        
        // Reset all frames first
        for (int i = 0; i < frameCount && i < MAX_FRAMES; i++) {
            // Start position of this frame in the texture
            int left = 0;
            for (int j = 0; j < i; j++) {
                left += (j < num_frames) ? widths[j] : originalFrameW;
            }
            
            // Width to use (either custom or original)
            int width = (i < num_frames) ? widths[i] : drawW;
            
            frames[i] = sf::IntRect(left, 0, width, originalFrameH);
        }
        
        // Reset current frame to apply new frame rect
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