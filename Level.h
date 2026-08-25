#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>
#include <string>

struct LevelConfig {
    int levelId;     
    int         width;        
    int         height;     
    int         cellSize;   
    std::string bgImage; 
    std::string musicFile;    
    float       gravity;     
    float       terminalVel; 
    float       maxSpeed;   
    float       acceleration; 
    float       deceleration; 
};
static constexpr int MAX_ROWS = 14;
static constexpr int MAX_COLS = 300;
#define MAX_CRABS 50

class Level {
public:
    Level(const LevelConfig& cfg);
    void initGridFromArray();
    bool collidesAt(float worldX, float worldY, bool isKnuckles) const;
    void render(sf::RenderWindow& window);


    void setGridCell(float x, float y, char value) ;
    char getGridCell(float x, float y) const ;

    int getWidth()    const { return w; }
    int getHeight()   const { return h; }
    int getCellSize() const { return cellSize; }

    int   getCrabCount()  const { return crabsCount; }
    float getCrabX(int i) const { return crabX[i]; }
    float getCrabY(int i) const { return crabY[i]; }
    void spawnCrabs();

    float getbugX(int i) { return bugX[i]; }
    float getbugY(int i) { return bugY[i]; }
    int getBugsCount() { return bugCount; }
    void spawnBugs();

    float getBatsX(int i) { return batX[i]; }
    float getBatsY(int i) { return batY[i]; }
    int getBatsCount() { return batCount; }
    void spawnBats();

    float getBuzzX(int i) { return buzzX[i]; }
    float getBuzzY(int i) { return buzzY[i]; }
    int getBuzzCount() { return buzzCount; }
    void spawnBUZZ();

	float getBossX() { return bossX; }
	float getBossY() { return bossY; }
	void spawnBOSS();

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

    static const int ringsCap = 50;
    static const int livesCap = 50;
    static const int boostCap = 50;
    static const int spikesCap = 50;
    static const int platformsCap = 50;
    static const int crabsCap = 50;
    static const int bugsCap = 50;
    static const int batsCap = 50;
    static const int buzzCap = 50;

    float ringX[ringsCap], ringY[ringsCap];
    int ringsCount;
    float livesX[livesCap], livesY[livesCap];
    int livesCount;
    float boostX[boostCap], boostY[boostCap];
    int boostCount;

    float spikesX[spikesCap], spikesY[spikesCap];
    int spikesCount;
    float platformsX[platformsCap], platformsY[platformsCap];
    int platformsCount;

    float crabX[crabsCap], crabY[crabsCap];
    int crabsCount;
    float bugX[bugsCap], bugY[bugsCap];
    int bugCount;
    float batX[batsCap], batY[batsCap];
    int batCount;
    float buzzX[buzzCap], buzzY[buzzCap];
    int buzzCount;
    float crabMinX[crabsCap], crabMaxX[crabsCap];
    int crabDir[crabsCap];
	float bossX, bossY;

    void loadTextures();
    void placeCollectibles(char type, int& count, int cap,
        float* xs, float* ys);
};


