#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Level.h"
#include "PlayerCharacter.h"
#include "Sonic.h"
#include "Tails.h"
#include "Knuckles.h"
#include "Enemy.h"
#include "Crab.h"
#include "MotoBug.h"
#include "BuzzBomber.h"
#include "BatBrain.h"
#include "Boss.h"
#include "Collectibles.h"
#include "Obstacles.h"

class Game {
public:
    Game(int screenX, int screenY, int levelNumber);
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void switchPlayer();
    void repositionAfterSwitch(PlayerCharacter* previousLeader);
    sf::RenderWindow* window;
    sf::View          view;
    Level* level;

    collectibles** rings;
    collectibles** lives;
    collectibles** boost;
    obstacles** spikes;
    obstacles** platforms;

    Sonic* sonic;
    Tails* tails;
    Knuckles* knuckles;
    PlayerCharacter* activePlayer;
    int               activeIndex;
    bool              lastDirRight;

    sf::Text menu[4];
    sf::Font font;
    sf::Text ringText;
    sf::Text scoreText;
    sf::Text lsText;

    EnemyCharacter* enemy;
    Crabmeat** crab;
    MotoBug** bug;
    BuzzBomber** buzzbomber;
    BatBrain** bat;
    Boss* boss;
    int crabCount;
    int ringCount;
    int bugCount;
    int batCount;
    int buzzCount;


    float currentCap;
    static constexpr float startSpeed = 2.f;
    static constexpr float sonicTarget = 30.f;
    static constexpr float followTarget = 20.f;
    static constexpr float rampDuration = 5.f;

    sf::Music         backgroundMusic;

    static LevelConfig getLevelConfig(int n);
};
