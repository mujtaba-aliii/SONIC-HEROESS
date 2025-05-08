#include "Level.h"
#include "Game.h"
#include <iostream>
#include <algorithm>

Level::Level(int height, int width, int cellSize)
    : h(height), w(width), cellSize(cellSize), crabCount(0)
{
    // initialize entire grid to spaces
    for (int i = 0; i < MAX_ROWS; ++i)
        for (int j = 0; j < MAX_COLS; ++j)
            grid[i][j] = ' ';

    // load wall texture
    if (!wallTexture.loadFromFile("Data/brick1.png"))
        std::cerr << "Error: Failed to load brick1.png\n";
    wallSprite.setTexture(wallTexture);

    // load background
    if (!backgroundTexture.loadFromFile("Data/bg1.png"))
        std::cerr << "Error: Failed to load background.png\n";
    backgroundSprite.setTexture(backgroundTexture);

    if (!crabTexture.loadFromFile("Data/crab.png"))
        std::cerr << "Error: Failed to load crab.png\n";
    crabSprite.setTexture(crabTexture);

    if (!ringTexture.loadFromFile("Data/ring.png"))
        std::cerr << "Error: Failed to load ring.png\n";
    ringSprite.setTexture(ringTexture);
    // scale background to cover the whole level area
    sf::Vector2u sz = backgroundTexture.getSize();
    backgroundSprite.setScale(
        float(w * cellSize) / sz.x,
        float(h * cellSize) / sz.y
    );
}

void Level::initGridFromArray() {
    // *** MAKE SURE WE HAVE EXACTLY 14 ROWS HERE! ***
    static const char* layout[MAX_ROWS] = {
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                              r  r                                                                                                                                                                     ",
        "                   r        wwwwwww                                                                                                                                                                    ",
        "                wwwwww                                                                                                                                                                                 ",
        "                                                       r                                                                                                                                               ",
        "                    c             c       r           ww        r         c                 c                                                                                                          ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };
    int rowsToCopy = std::min(h, MAX_ROWS);
    for (int i = 0; i < rowsToCopy; ++i) {
        int len = std::min(w, int(std::strlen(layout[i])));
        for (int j = 0; j < len; ++j) {
            grid[i][j] = layout[i][j];
        }
    }
}

bool Level::collidesAt(float worldX, float worldY) const {
    int i = int(worldY) / cellSize;
    int j = int(worldX) / cellSize;
    if (i < 0 || i >= h || j < 0 || j >= w) return false;
    return grid[i][j] == 'w';
}

void Level::spawnCrabs() {
    crabCount = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'c') {
                if (crabCount < MAX_CRABS) {
                    crabX[crabCount] = j * cellSize;
                    crabY[crabCount] = i * cellSize;
                    crabCount++;

                }
                grid[i][j] = ' ';
            }

        }
    }
}

void Level::ringsPlacement() {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'r') {
                if (ringCount < MAX_RINGS) {
                    ringX[ringCount] = j * cellSize;
                    ringY[ringCount] = i * cellSize;
                    ringCount++;
                    grid[i][j] = ' ';
                }

            }
        }
    }
}

void Level::render(sf::RenderWindow& window) {
    // draw background first
    window.draw(backgroundSprite);

    // compute which tiles are visible in the current view
    sf::FloatRect viewRect(
        window.getView().getCenter() - window.getView().getSize() / 2.f,
        window.getView().getSize()
    );

    int startI = std::max(0, int(viewRect.top) / cellSize);
    int endI = std::min(h - 1, int((viewRect.top + viewRect.height)) / cellSize);
    int startJ = std::max(0, int(viewRect.left) / cellSize);
    int endJ = std::min(w - 1, int((viewRect.left + viewRect.width)) / cellSize);

    for (int i = startI; i <= endI; ++i) {
        for (int j = startJ; j <= endJ; ++j) {
            if (grid[i][j] == 'w') {
                wallSprite.setPosition(float(j * cellSize), float(i * cellSize));
                window.draw(wallSprite);
            }
        }
    }
}
