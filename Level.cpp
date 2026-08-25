#include "Level.h"
#include <fstream>
#include <iostream>
#include <cstring>

Level::Level(const LevelConfig& cfg)
    : config(cfg),
    h(cfg.height),
    w(cfg.width),
    cellSize(cfg.cellSize),
    crabsCount(0),
    ringsCount(0),
    livesCount(0),
    boostCount(0),
    spikesCount(0),
    platformsCount(0)
{
    loadTextures();

    sf::Vector2u bgSz = backgroundTexture.getSize();
    backgroundSprite.setScale(
        float(w * cellSize) / bgSz.x,
        float(h * cellSize) / bgSz.y
    );

    initGridFromArray();
}

void Level::initGridFromArray() {
    static const char* layout1[200] = {
        "                                                                                                                                                                                                       ",
        "                                                                                                                                                                                                       ",
        "                B                                                                                                                                                                                      ",
        "                                                                                                                                                                                                       ",
        "                                   B                            B                                                                      b                                                               ",
        "       b                                                                                  B                                                                           r                                ",
        "                                                                               b                                                              B                     ppp                                ",
        "                                                                                                                                                                                                       ",
        "                              r  r         r                                 r                                             rr                                                                          ",
        "                   r        ppppppp                                  r      pppp               f                      l  pppppp                                                                        ",
        "                pppppp                                              ppppp                     ppp                   ppp                               f                                                ",
        "                                            w          r       ppp                                                w                                  ppp                                               ",
        "                  s   l  kkkk     c         w         ww        l         m         s       c  l        r         w              f                                                                     ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    static const char* layout2[250] = {
        "                                                                                                                                                                                                                                                         ",
        "                                                                                                                                                                                                                                                         ",
        "                B                                                                                                                                                                                                                                        ",
        "                                                                                                                                                                                                         b                                               ",
        "                                   B                            B                                                                      b                                   B                                                                             ",
        "       b                                                                                  B                                                                           r                                                                                  ",
        "                                                                               b                                                              B                     ppp                                                     B                            ",
        "                                                                                                                                                                                              b                                    b                     ",
        "                              r  r         r                                 r                                             rr                                                                                                                            ",
        "                   r        ppppppp                                  r      pppp               f                      l  pppppp                                                       rrr              l                                                 ",
        "                pppppp                                              ppppp                     ppp                    ppp                               f                             pppp           pppp                                                 ",
        "                                                       r       ppp                                              w                                    ppp                                                            ppp                                  ",
        "                  s      kkkk     c       w           ww        l         m         s       c  l        r       w  m        f                c                    c      l              r    m     f            r       c       l    w       m           ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    static const char* layout3[300] = {
        "                                                                                                                                                                                                                                                                                                           ",
        "                                                                                                                                                                                                                                                                                                           ",
        "                B                                                                                                                                                                                                                     b                                                                    ",
        "                                                                                                                b                                      B                                                 b                                                                                                 ",
        "                                   B                            B                                                                      b                                   B                                                                                                                               ",
        "       b                                                                                  B                                                                           r                                                                                                                      B             ",
        "                                                                               b                                                              B                     ppp                                                     B                          B                                                   ",
        "                                                  B                                                                                                                                           b                                    b                                           b                           ",
        "                              r  r         r                                 r                                             rr                                                                                                                                                                              ",
        "                   r        ppppppp                                  r      pppp               f pppp                 l  pppppp                                                       rrr              l                                                                                                   ",
        "                pppppp                                              ppppp                     ppp                    ppp                               f                             pppp           pppp                                                       w                                           ",
        "                                                       r       ppp               w                            w                                      ppp                                                            ppp                                        w                                           ",
        "                  s      kkkk     c       w      m    ww        l         m      w  s       c  l        r     w    m        f       m        c                    c      l              r    m     f            r       c       l             m                w               ppp         m      c        ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww       wwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    static const char* layout4[40] = {
        "                                               ",
        "                                               ",
        "                                               ",
        "                                               ",
        "                                               ",
        "          E                                    ",
        "                                               ",
        "                                               ",
        "                                               ",
        "      ppp                   pppp               ",
        " pppp           pppppp                         ",
        "                         pp                    ",
        "                                               ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

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

    int rowsToCopy = std::min(h, MAX_ROWS);
    for (int i = 0; i < rowsToCopy; ++i) {
        int len = std::min(w, int(std::strlen(selectedLayout[i])));
        for (int j = 0; j < len; ++j) {
            grid[i][j] = selectedLayout[i][j];
        }
    }
}
char Level::getGridCell(float worldX, float worldY) const {
    int i = int(worldY) / cellSize;
    int j = int(worldX) / cellSize;
    if (i < 0 || i >= h || j < 0 || j >= w) return ' ';
    return grid[i][j];
}

void Level::setGridCell(float worldX, float worldY, char value) {
    int i = int(worldY) / cellSize;
    int j = int(worldX) / cellSize;
    if (i >= 0 && i < h && j >= 0 && j < w) {
        grid[i][j] = value;
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

bool Level::collidesAt(float worldX, float worldY, bool isKnuckles) const {
    int i = int(worldY) / cellSize;
    int j = int(worldX) / cellSize;
    if (i < 0 || i >= h || j < 0 || j >= w) return false;

    // If the character is Knuckles, allow bypassing 'k' walls
    if (isKnuckles && grid[i][j] == 'k') return false;

    // Treat 'w' and 'k' as solid for all other characters
    return (grid[i][j] == 'w' || grid[i][j] == 'k');
}


void Level::spawnCrabs() {
    crabsCount = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'c') {
                    float x = j * cellSize;
                    float y = i * cellSize;

                    crabX[crabsCount] = x;
                    crabY[crabsCount] = y;

                    // Set patrol bounds to 2 tiles left and right
                    crabMinX[crabsCount] = x - cellSize * 2;
                    crabMaxX[crabsCount] = x + cellSize * 2;
                    crabDir[crabsCount] = 1; // Start moving right

                    crabsCount++;
                grid[i][j] = ' ';
            }
        }
    }
}

void Level::spawnBugs() {
    bugCount = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'm') {
                    bugX[bugCount] = j * cellSize;
                    bugY[bugCount] = i * cellSize;
                    bugCount++;
                grid[i][j] = ' ';
            }

        }
    }
}
void Level::spawnBats() {
    batCount = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'b') {
                    batX[batCount] = j * cellSize;
                    batY[batCount] = i * cellSize;
                    batCount++;
                grid[i][j] = ' ';
            }

        }
    }
}
void Level::spawnBUZZ() {
    buzzCount = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'B') {
                    buzzX[buzzCount] = j * cellSize;
                    buzzY[buzzCount] = i * cellSize;
                    buzzCount++;
                grid[i][j] = ' ';
            }

        }
    }
}

void Level::spawnBOSS() {
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == 'E') {
                bossX = j * cellSize;
                bossY = i * cellSize;

                grid[i][j] = ' ';

                return;
            }
        }
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


void Level::placeCollectibles(char type, int& count, int cap, float* xs, float* ys) {
    for (int i = 0;i < h;++i)
      for (int j = 0;j < w;++j)
        if (grid[i][j] == type && count < cap) {
            xs[count] = j * cellSize;
            ys[count] = i * cellSize;
            count++;
            grid[i][j] = ' ';
        }
}

void Level::ringsPlacement() { placeCollectibles('r', ringsCount, ringsCap, ringX, ringY); }
void Level::livesPlacement() { placeCollectibles('l', livesCount, livesCap, livesX, livesY); }
void Level::boostPlacement() { placeCollectibles('f', boostCount, boostCap, boostX, boostY); }

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

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
           if (grid[i][j] == 'w' || grid[i][j] == 'k') {
                wallSprite.setPosition(float(j * cellSize), float(i * cellSize));
                window.draw(wallSprite);
            }
        }
    }
}

