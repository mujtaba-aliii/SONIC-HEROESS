//#include "Level.h"
//#include <iostream>
//#include <algorithm>
//Level::Level(int height, int width, int cellSize)
//    : h(height), w(width), cellSize(cellSize)
//    , grid(h, std::vector<char>(w, ' '))
//    , crabCount(0)  // Fix: initialize crabCount
//{
//    if (!wallTexture.loadFromFile("Data/brick1.png"))
//        std::cerr << "Error: Failed to load brick1.png\n";
//    wallSprite.setTexture(wallTexture);
//
//    if (!crabTexture.loadFromFile("Data/crab.png"))  // Fix: load crab texture
//        std::cerr << "Error: Failed to load crab.png\n";
//    crabSprite.setTexture(crabTexture);              // Fix: set crab sprite texture
//}
//
//
//void Level::initGridFromArray() {
//    // Modified level layout with a visible ground row
//    static const std::vector<std::string> layout = {
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                                                                                                              ",
//        "                    c             c                  ww                 c                 c                   ",
//        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
//    };
//
//    if (layout.size() != size_t(h)) {
//        std::cerr
//            << "Warning: layout has " << layout.size()
//            << " rows but Level height is " << h << "\n";
//    }
//
//    int rowsToCopy = std::min((int)layout.size(), h);
//    for (int i = 0; i < rowsToCopy; ++i) {
//        if ((int)layout[i].size() != w) {
//            std::cerr
//                << "Warning: layout[" << i << "] has length "
//                << layout[i].size() << " but Level width is " << w << "\n";
//        }
//        int colsToCopy = std::min((int)layout[i].size(), w);
//        for (int j = 0; j < colsToCopy; ++j) {
//            grid[i][j] = layout[i][j];
//        }
//        // any j>=colsToCopy remain ' ' by default
//    }
//    // any i>=rowsToCopy remain all ' '
//}
//
//bool Level::collidesAt(float worldX, float worldY) const {
//    int i = int(worldY) / cellSize;
//    int j = int(worldX) / cellSize;
//
//    // Bounds checking
//    if (i < 0 || i >= h || j < 0 || j >= w) return false;
//
//    return grid[i][j] == 'w';
//}
//void Level::spawnCrabs() {
//    crabCount = 0;
//
//    for (int i = 0; i < h; ++i) {
//        for (int j = 0; j < w; ++j) {
//            if (grid[i][j] == 'c') {
//                if (crabCount < MAX_CRABS) {
//                    crabX[crabCount] = j * cellSize;
//                    crabY[crabCount] = i * cellSize;
//                    crabCount++;
//                }
//                // Optional: replace 'c' with ' ' so it's not interpreted as terrain
//                grid[i][j] = ' ';
//            }
//        }
//    }
//}
//
//void Level::render(sf::RenderWindow& window) {
//    // Only render cells that are within the view area
//    sf::FloatRect viewRect = sf::FloatRect(window.getView().getCenter() - window.getView().getSize() / 2.f, window.getView().getSize());
//
//    // Determine visible grid bounds
//    int startI = std::max(0, int(viewRect.top) / cellSize);
//    int endI = std::min(h - 1, int(viewRect.top + viewRect.height) / cellSize);
//    int startJ = std::max(0, int(viewRect.left) / cellSize);
//    int endJ = std::min(w - 1, int(viewRect.left + viewRect.width) / cellSize);
//
//    // Render only visible cells
//    for (int i = startI; i <= endI; ++i) {
//        for (int j = startJ; j <= endJ; ++j) {
//            if (grid[i][j] == 'w') {
//                wallSprite.setPosition(float(j * cellSize), float(i * cellSize));
//                window.draw(wallSprite);
//            }
//        }
//    }
//    for (int i = 0; i < crabCount; ++i) {
//        crabSprite.setPosition(crabX[i], crabY[i]);
//        window.draw(crabSprite);
//    }
//
//}
#include "Level.h"
#include <iostream>
#include <algorithm>
#include "Game.h"

Level::Level(int height, int width, int cellSize)
    : h(height), w(width), cellSize(cellSize), crabCount(0), ringCount(0)
{
    if (!wallTexture.loadFromFile("Data/brick1.png"))
        std::cerr << "Error: Failed to load brick1.png\n";
    wallSprite.setTexture(wallTexture);

    if (!crabTexture.loadFromFile("Data/crab.png"))
        std::cerr << "Error: Failed to load crab.png\n";
    crabSprite.setTexture(crabTexture);

    if (!ringTexture.loadFromFile("Data/ring.png"))
        std::cerr << "Error: Failed to load ring.png\n";
    ringSprite.setTexture(ringTexture);

    // Initialize grid to spaces
    for (int i = 0; i < MAX_HEIGHT; ++i)
        for (int j = 0; j < MAX_WIDTH; ++j)
            grid[i][j] = ' ';
}

void Level::initGridFromArray() {
    static const std::vector<std::string> layout = {
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                                                                                                              ",
        "                              r  r                                                                            ",
        "                   r        wwwwwww                                                                           ",
        "                wwwwww                                                                                        ",
        "                                                       r                                                      ",
        "                    c             c       r           ww        r         c                 c                 ",
        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };

    for (int i = 0; i < h && i < 14; ++i) {
        for (int j = 0; j < w && j < 102; ++j) {
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
    sf::FloatRect viewRect = sf::FloatRect(window.getView().getCenter() - window.getView().getSize() / 2.f, window.getView().getSize());

    int startI = std::max(0, int(viewRect.top) / cellSize);
    int endI = std::min(h - 1, int(viewRect.top + viewRect.height) / cellSize);
    int startJ = std::max(0, int(viewRect.left) / cellSize);
    int endJ = std::min(w - 1, int(viewRect.left + viewRect.width) / cellSize);

    for (int i = startI; i <= endI; ++i) {
        for (int j = startJ; j <= endJ; ++j) {
            if (grid[i][j] == 'w') {
                wallSprite.setPosition(float(j * cellSize), float(i * cellSize));
                window.draw(wallSprite);

            }
        }
    }

    
}
