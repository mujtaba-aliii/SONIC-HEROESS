//// Level.h
//#pragma once
//
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <string>
//
//class Level {
//public:
//    Level(int height, int width, int cellSize);
//
//    // leave these public so Game can call them
//    void initGridFromArray();
//    bool collidesAt(float worldX, float worldY) const;
//    void render(sf::RenderWindow& window);
//    int getWidth()    const { return w; }
//    int getHeight()   const { return h; }
//    int getCellSize() const { return cellSize; }
//    void spawnCrabs();
//private:
//    int h, w, cellSize;
//    std::vector<std::vector<char>> grid;
//    sf::Texture wallTexture;
//    sf::Sprite  wallSprite;
//   
//    static const int MAX_CRABS = 10;
//
//    sf::Texture crabTexture;
//    sf::Sprite crabSprite;
//    float crabX[MAX_CRABS];  // world X coordinates
//    float crabY[MAX_CRABS];  // world Y coordinates
//    int crabCount;           // number of crabs actually used
//
//};

#pragma once
#include <SFML/Graphics.hpp>

#define MAX_WIDTH 103
#define MAX_HEIGHT 20
#define MAX_CRABS 50

class Level {
public:
    Level(int height, int width, int cellSize);

    void initGridFromArray();
    void spawnCrabs();
    void ringsPlacement();
    void render(sf::RenderWindow& window);
    bool collidesAt(float worldX, float worldY) const;

    int getCellSize() const { return cellSize; }
    int getWidth() const { return w; }
    int getHeight() const { return h; }

    float getCrabX(int i) {return crabX[i];}

    float getCrabY(int i) { return crabY[i];}
    float getRingX(int i) { return ringX[i]; }

    float getRingY(int i) { return ringY[i]; }

    int getCrabCount() { return crabCount; }
    int getRingsCount() { return ringCount; }
    static const int MAX_RINGS = 50;

    int ringX[MAX_RINGS];
    int ringY[MAX_RINGS];
    int ringCount;

    int crabX[MAX_CRABS];
    int crabY[MAX_CRABS];
    int crabCount;
    char grid[MAX_HEIGHT][MAX_WIDTH];

private:
    int h, w;
    int cellSize;

    
    sf::Texture wallTexture;
    sf::Sprite wallSprite;

    sf::Texture crabTexture;
    sf::Sprite crabSprite;
    
    sf::Texture ringTexture;
    sf::Sprite  ringSprite;

};
