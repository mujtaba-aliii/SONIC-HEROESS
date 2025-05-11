#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>
#include <string>

struct LevelConfig {
    int levelId;      // 1, 2, 3, 4
    int         width;        // cells in X
    int         height;       // cells in Y
    int         cellSize;     // pixels per cell
    std::string bgImage;      // e.g. "Data/bg1.png"
    std::string musicFile;    // e.g. "Data/labrynth1.wav"
    float       gravity;      // gravity constant
    float       terminalVel;  // max fall speed
    float       maxSpeed;     // horizontal top speed
    float       acceleration; // accel rate
    float       deceleration; // decel rate
};
static constexpr int MAX_ROWS = 14;
static constexpr int MAX_COLS = 300;
#define MAX_CRABS 50

class Level {
public:
    Level(const LevelConfig& cfg);
    void initGridFromArray();
    bool collidesAt(float worldX, float worldY) const;
    void render(sf::RenderWindow& window);

    int getWidth()    const { return w; }
    int getHeight()   const { return h; }
    int getCellSize() const { return cellSize; }

    int   getCrabCount()  const { return crabCount; }
    float getCrabX(int i) const { return crabX[i]; }
    float getCrabY(int i) const { return crabY[i]; }
    void spawnCrabs();

    int   getRingsCount() const { return ringsCount; }
    float getRingX(int i)  const { return ringX[i]; }
    float getRingY(int i)  const { return ringY[i]; }
    void ringsPlacement();

    int   getLivesCount() const { return livesCount; }
    float getLivesX(int i) const { return livesX[i]; }
    float getLivesY(int i) const { return livesY[i]; }
    void livesPlacement();

    int   getBoostCount() const { return boostCount; }
    float getBoostX(int i) const { return boostX[i]; }
    float getBoostY(int i) const { return boostY[i]; }
    void boostPlacement();

    int   getSpikesCount() const { return spikesCount; }
    float getSpikesX(int i)  const { return spikesX[i]; }
    float getSpikesY(int i)  const { return spikesY[i]; }
    void spawnSpikes();

    int   getPlatformsCount() const { return platformsCount; }
    float getPlatformsX(int i) const { return platformsX[i]; }
    float getPlatformsY(int i) const { return platformsY[i]; }
    void spawnPlatforms();

    void renderCollectibles(sf::RenderWindow& window,
        int count,
        const sf::Texture& tex,
        const int* xs,
        const int* ys);

private:
    LevelConfig config;
    int         h, w, cellSize;
    char        grid[MAX_ROWS][MAX_COLS];

    sf::Texture wallTexture;       sf::Sprite wallSprite;
    sf::Texture backgroundTexture; sf::Sprite backgroundSprite;
    sf::Music   backgroundMusic;
    sf::Texture crabTexture;
    sf::Texture ringTexture;
    sf::Texture livesTexture;
    sf::Texture boostTexture;

    int crabX[MAX_CRABS], crabY[MAX_CRABS], crabCount;

    static const int ringsCap = 50;
    static const int livesCap = 50;
    static const int boostCap = 50;
    static const int spikesCap = 50;
    static const int platformsCap = 50;

    int ringX[ringsCap], ringY[ringsCap], ringsCount;
    int livesX[livesCap], livesY[livesCap], livesCount;
    int boostX[boostCap], boostY[boostCap], boostCount;

    int spikesX[spikesCap], spikesY[spikesCap], spikesCount;
    int platformsX[platformsCap], platformsY[platformsCap], platformsCount;

    void loadTextures();
    void placeCollectibles(char type, int& count, int cap,
        int* xs, int* ys);
};


