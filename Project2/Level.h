#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>  // for strlen

static constexpr int MAX_ROWS = 14;
static constexpr int MAX_COLS = 200;

class Level {
public:
    Level(int height, int width, int cellSize);
    void initGridFromArray();
    bool collidesAt(float worldX, float worldY) const;
    void render(sf::RenderWindow& window);

    int getWidth()  const { return w; }
    int getHeight() const { return h; }
    int getCellSize() const { return cellSize; }

private:
    int h, w, cellSize;
    char grid[MAX_ROWS][MAX_COLS];
    sf::Texture wallTexture;
    sf::Sprite  wallSprite;
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
};
