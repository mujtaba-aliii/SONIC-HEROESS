#pragma once

#include "Level.h"
#include "PlayerCharacter.h"
#include "Sonic.h"
#include "Tails.h"
#include "Knuckles.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Crab.h"
#include "MotoBug.h"
#include "BuzzBomber.h"
#include "BatBrain.h"
#include "Collectibles.h"

class Game {
public:
    Game(int screenX, int screenY);
    ~Game();
    void run();
    void notifyGlidingState(bool isGliding);
private:
    void processEvents();
    void update(float dt);
    void render();
    void switchPlayer();
    void repositionAfterSwitch(PlayerCharacter* previousLeader);


    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::RenderWindow* window;
    sf::View          view;
    Level             level;

    collectibles** rings;
    Sonic* sonic;
    Tails* tails;
    Knuckles* knuckles;
    PlayerCharacter* activePlayer;
    int               activeIndex;
    bool              lastDirRight;
    EnemyCharacter* enemy;
    Crabmeat** crab;
    MotoBug* motobug;
    BuzzBomber* buzzbomber;
    BatBrain* batbrain;
    int crabCount;
    int ringCount;
    // ─── build‑up speed cap ─────────────────────────────────────
    float currentCap;    // ramps each frame from startSpeed → targetCap

    static constexpr float startSpeed = 2.f;
    static constexpr float sonicTarget = 30.f;
    static constexpr float followTarget = 20.f;
    static constexpr float rampDuration = 5.f;  // seconds to reach target
    // ──────────────────────────────────────────────────────────

    static constexpr float formationSpacing = 70.f;

    sf::Music         backgroundMusic;
};
