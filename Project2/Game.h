#pragma once

#include "Level.h"
#include "PlayerCharacter.h"
#include "Sonic.h"
#include "Tails.h"
#include "Knuckles.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

    sf::RenderWindow* window;
    sf::View          view;
    Level             level;

    Sonic* sonic;
    Tails* tails;
    Knuckles* knuckles;
    PlayerCharacter* activePlayer;
    int               activeIndex;
    bool              lastDirRight;

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
