#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>  // for strlen

static constexpr int MAX_ROWS = 14;
static constexpr int MAX_COLS = 200;
#define MAX_CRABS 50
class Level {
public:
    Level(int height, int width, int cellSize);
    void initGridFromArray();
    bool collidesAt(float worldX, float worldY) const;
    void render(sf::RenderWindow& window);

    int getWidth()  const { return w; }
    int getHeight() const { return h; }
    int getCellSize() const { return cellSize; }

    float getCrabX(int i) { return crabX[i]; }

    float getCrabY(int i) { return crabY[i]; }

    float getRingX(int i) { return ringX[i]; }

    float getRingY(int i) { return ringY[i]; }
    int getCrabCount() { return crabCount; }
    int getRingsCount() { return ringCount; }
    void spawnCrabs();
    void ringsPlacement();static const int MAX_RINGS = 50;
    int ringX[MAX_RINGS];
    int ringY[MAX_RINGS];
    int ringCount;
    int crabX[MAX_CRABS];
    int crabY[MAX_CRABS];
    int crabCount;

private:
    int h, w, cellSize;
    char grid[MAX_ROWS][MAX_COLS];
    sf::Texture wallTexture;
    sf::Sprite  wallSprite;
    sf::Texture crabTexture;
    sf::Sprite  crabSprite;
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Texture ringTexture;
    sf::Sprite  ringSprite;
};
