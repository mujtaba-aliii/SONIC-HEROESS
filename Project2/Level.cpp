#include "Level.h"
#include <fstream>
#include <iostream>
#include <cstring>

Level::Level(const LevelConfig& cfg)
    : config(cfg),
    h(cfg.height),
    w(cfg.width),
    cellSize(cfg.cellSize),
    crabCount(0),
    ringsCount(0),
    livesCount(0),
    boostCount(0),
    spikesCount(0),
    platformsCount(0)
{
    loadTextures();

    // Scale background to cover level
    sf::Vector2u bgSz = backgroundTexture.getSize();
    backgroundSprite.setScale(
        float(w * cellSize) / bgSz.x,
        float(h * cellSize) / bgSz.y
    );

    // Load the grid layout based on levelId
    initGridFromArray();
}

void Level::initGridFromArray() {
    // Define layouts for different levels
    static const char* layout1[200] = {
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                              r  r                                                                                                                                                                     ",
        "                   r        ppppppp                                                                                                                                                                    ",
        "                pppppp                    w                                                                                                                                                            ",
        "                                          w            r       ppp                                                                                                                                     ",
        "         s          b       s     c       w           ww        l         b         s       c                                                                                                          ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    static const char* layout2[250] = {
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                              r  r                                                                                                                                                                     ",
        "                   r        ppppppp                                                                                                                                                                    ",
        "                pppppp                    w                                                                                                                                                            ",
        "                                          w            r       ppp                                                                                                                                     ",
        "         s          b       s     c       w           ww        l         b         s       c                                                                                                          ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    static const char* layout3[300] = {
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                              r  r                                                                                                                                                                     ",
        "                   r        ppppppp                                                                                                                                                                    ",
        "                pppppp                    w                                                                                                                                                            ",
        "                                          w            r       ppp                                                                                                                                     ",
        "         s          b       s     c       w           ww        l         b         s       c                                                                                                          ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    static const char* layout4[20] = {
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                              r  r                                                                                                                                                                     ",
        "                   r        ppppppp                                                                                                                                                                    ",
        "                pppppp                    w                                                                                                                                                            ",
        "                                          w            r       ppp                                                                                                                                     ",
        "         s          b       s     c       w           ww        l         b         s       c                                                                                                          ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    // Select the appropriate layout based on levelId
    const char** selectedLayout = nullptr;
    switch (config.levelId) {
    case 1: selectedLayout = layout1; break;
    case 2: selectedLayout = layout2; break;
    case 3: selectedLayout = layout3; break;
    case 4: selectedLayout = layout4; break;
    default:
        std::cerr << "Invalid levelId: " << config.levelId << ". Defaulting to layout1.\n";
        selectedLayout = layout1;
        break;
    }

    // Copy the selected layout into the grid
    int rowsToCopy = std::min(h, MAX_ROWS);
    for (int i = 0; i < rowsToCopy; ++i) {
        int len = std::min(w, int(std::strlen(selectedLayout[i])));
        for (int j = 0; j < len; ++j) {
            grid[i][j] = selectedLayout[i][j];
        }
    }
}


void Level::loadTextures() {
    if (!wallTexture.loadFromFile("Data/brick1.png"))
        std::cerr << "Error loading brick1.png\n";
    wallSprite.setTexture(wallTexture);

    if (!backgroundTexture.loadFromFile(config.bgImage))
        std::cerr << "Error loading " << config.bgImage << "\n";
    backgroundSprite.setTexture(backgroundTexture);

    if (!backgroundMusic.openFromFile(config.musicFile))
        std::cerr << "Error loading " << config.musicFile << "\n";
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(30.f);
    backgroundMusic.play();

    if (!crabTexture.loadFromFile("Data/crab.png"))
        std::cerr << "Error loading crab.png\n";
    if (!ringTexture.loadFromFile("Data/ring.png"))
        std::cerr << "Error loading ring.png\n";
    if (!livesTexture.loadFromFile("Data/crystal.png"))
        std::cerr << "Error loading crystal.png\n";
    if (!boostTexture.loadFromFile("Data/crabstill.png"))
        std::cerr << "Error loading crabstill.png\n";
}

bool Level::collidesAt(float worldX, float worldY) const {
    int i = int(worldY) / cellSize;
    int j = int(worldX) / cellSize;
    if (i < 0 || i >= h || j < 0 || j >= w) return false;
    return grid[i][j] == 'w';
}

void Level::spawnCrabs() {
    crabCount = 0;
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            if (grid[i][j] == 'c' && crabCount < MAX_CRABS) {
                crabX[crabCount] = j * cellSize;
                crabY[crabCount] = i * cellSize;
                crabCount++;
                grid[i][j] = ' ';
            }
}

void Level::spawnSpikes() {
    spikesCount = 0;
    for (int i = 0;i < h;++i) for (int j = 0;j < w;++j)
        if (grid[i][j] == 's' && spikesCount < spikesCap) {
            spikesX[spikesCount] = j * cellSize;
            spikesY[spikesCount] = i * cellSize;
            spikesCount++;
            grid[i][j] = ' ';
        }
}

void Level::spawnPlatforms() {
    platformsCount = 0;
    for (int i = 0;i < h;++i) for (int j = 0;j < w;++j)
        if (grid[i][j] == 'p' && platformsCount < platformsCap) {
            platformsX[platformsCount] = j * cellSize;
            platformsY[platformsCount] = i * cellSize;
            platformsCount++;
            grid[i][j] = ' ';
        }
}

void Level::placeCollectibles(char type, int& count, int cap, int* xs, int* ys) {
    for (int i = 0;i < h;++i)for (int j = 0;j < w;++j)
        if (grid[i][j] == type && count < cap) {
            xs[count] = j * cellSize;
            ys[count] = i * cellSize;
            count++;
            grid[i][j] = ' ';
        }
}

void Level::ringsPlacement() { placeCollectibles('r', ringsCount, ringsCap, ringX, ringY); }
void Level::livesPlacement() { placeCollectibles('l', livesCount, livesCap, livesX, livesY); }
void Level::boostPlacement() { placeCollectibles('b', boostCount, boostCap, boostX, boostY); }

void Level::renderCollectibles(sf::RenderWindow& w, int cnt, const sf::Texture& tex, const int* xs, const int* ys) {
    sf::Sprite s; s.setTexture(tex);
    auto tsz = tex.getSize();
    s.setScale(float(cellSize) / tsz.x, float(cellSize) / tsz.y);
    for (int i = 0;i < cnt;++i) {
        s.setPosition(xs[i], ys[i]);
        w.draw(s);
    }
}

void Level::render(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    auto vr = sf::FloatRect(
        window.getView().getCenter() - window.getView().getSize() / 2.f,
        window.getView().getSize()
    );
    int i0 = std::max(0, int(vr.top) / cellSize),
        i1 = std::min(h - 1, int((vr.top + vr.height)) / cellSize),
        j0 = std::max(0, int(vr.left) / cellSize),
        j1 = std::min(w - 1, int((vr.left + vr.width)) / cellSize);

    for (int i = i0;i <= i1;++i)
        for (int j = j0;j <= j1;++j)
            if (grid[i][j] == 'w') {
                wallSprite.setPosition(j * cellSize, i * cellSize);
                window.draw(wallSprite);
            }
}
