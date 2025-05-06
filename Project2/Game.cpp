// File: Game.cpp

#include "Game.h"
#include <iostream>
#include <algorithm>  // for std::min and std::max

Game::Game(int screenX, int screenY)
    : window(new sf::RenderWindow(sf::VideoMode(screenX, screenY), "Sonic the Hedgehog - OOP"))
    , view(sf::FloatRect(0, 0, (float)screenX, (float)screenY))
    , level(14, 200, 64)
    , activeIndex(0)
    , lastDirRight(true)
    , currentCap(startSpeed)
{
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    window->setView(view);
    level.initGridFromArray();

    sonic = new Sonic(0, 0);
    tails = new Tails(0, 0);
    knuckles = new Knuckles(0, 0);

    sonic->setScale(2.5f, 2.5f);
    tails->setScale(2.5f, 2.5f);
    knuckles->setScale(2.5f, 2.5f);

    float groundY = (level.getHeight() - 1) * level.getCellSize();
    knuckles->setPosition(100.f, groundY - knuckles->getSpriteHeight());
    tails->setPosition(170.f, groundY - tails->getSpriteHeight());
    sonic->setPosition(240.f, groundY - sonic->getSpriteHeight());

    activePlayer = sonic;
    sonic->toggleDebug();

    if (!backgroundMusic.openFromFile("Data/labrynth.ogg"))
        std::cerr << "Failed to load music\n";
    else {
        backgroundMusic.setVolume(30.f);
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }

    std::cout << "Game initialized. Use arrow keys to move, Up to jump." << std::endl;
}

Game::~Game() {
    delete sonic;
    delete tails;
    delete knuckles;
    delete window;
}

void Game::run() {
    sf::Clock clock;
    const float maxDt = 0.1f;

    while (window->isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, maxDt);
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event ev;
    while (window->pollEvent(ev)) {
        if (ev.type == sf::Event::Closed ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window->close();
        }
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Z)
            switchPlayer();
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::F1)
            activePlayer->toggleDebug();
        activePlayer->handleInput(ev);
    }
}

void Game::update(float dt) {
    float safeDt = std::max(dt, 0.0001f);

    int windowWidth = window->getSize().x;
    int windowHeight = window->getSize().y;
    int levelWidth = level.getWidth() * level.getCellSize();

    // 1) adjust speed cap
    if (activePlayer->isMoving()) {
        float targetCap = (activePlayer == sonic ? sonicTarget : followTarget);
        float rampRate = (targetCap - startSpeed) / rampDuration;
        currentCap = std::min(targetCap, currentCap + rampRate * safeDt);
    }
    else {
        currentCap = startSpeed;
    }

    // 2) apply to all
    sonic->setMaxSpeed(currentCap);
    tails->setMaxSpeed(currentCap);
    knuckles->setMaxSpeed(currentCap);

    // 3) update leader
    activePlayer->update(safeDt, level, windowWidth, windowHeight);

    // 4) leader info
    sf::Vector2f activePos = activePlayer->getPosition();
    float leaderVx = activePlayer->getVelocityX();

    // 5) facing
    constexpr float faceThresh = 0.1f;
    if (leaderVx > faceThresh)  lastDirRight = true;
    if (leaderVx < -faceThresh) lastDirRight = false;

    // 6) spacing (unused when gliding)
    constexpr float idleSpacing = formationSpacing;
    constexpr float movingSpacing = 100.f;
    float spacing = (std::abs(leaderVx) > faceThresh ? movingSpacing : idleSpacing);
    float doubleSpacing = spacing * 2.f;

    // 7) follower pointers
    PlayerCharacter* f1;
    PlayerCharacter* f2;
    if (activePlayer == sonic) { f1 = tails;    f2 = knuckles; }
    else if (activePlayer == tails) { f1 = knuckles; f2 = sonic; }
    else { f1 = sonic;    f2 = tails; }

    // 8) handle Tails’ gliding
    if (tails->isGlidePhaseActive()) {
        // Get Tails’ position
        sf::Vector2f tailPos = tails->getPosition();
        float tx = tailPos.x;
        float ty = tailPos.y;

        // Which way is “behind”? If facing right, behind = left (-1), else = right (+1)
        float dir = lastDirRight ? 1.f : -1.f;
        sonic->setPosition(tx - dir * 49.f, ty - 42.f);
        sonic->forceHang(lastDirRight);

        // --- Knuckles stays fixed behind Sonic ---
        sf::Vector2f sonicPos = sonic->getPosition();
        float sx = sonicPos.x;
        float sy = sonicPos.y;

        knuckles->setPosition(sx - dir * 49.f, sy + 44.f);
        knuckles->forceHang(lastDirRight);
    }
    else {
        // … your normal followTarget logic for f1 and f2 …
        if (lastDirRight) {
            f1->followTarget(activePos.x - spacing, activePos.y, leaderVx, safeDt, level);
            f2->followTarget(activePos.x - doubleSpacing, activePos.y, leaderVx, safeDt, level);
        }
        else {
            f1->followTarget(activePos.x + spacing, activePos.y, leaderVx, safeDt, level);
            f2->followTarget(activePos.x + doubleSpacing, activePos.y, leaderVx, safeDt, level);
        }
    }


    // 9) clamp followers
    f1->clampPosition(windowWidth, windowHeight, levelWidth);
    f2->clampPosition(windowWidth, windowHeight, levelWidth);

    // 10) camera
    sf::Vector2f camPos = activePos;
    float halfW = view.getSize().x / 2.f;
    float levelRight = levelWidth - halfW;
    camPos.x = std::max(halfW, std::min(camPos.x, levelRight));
    view.setCenter(camPos.x, view.getCenter().y);
    window->setView(view);
}

void Game::render() {
    window->clear();
    level.render(*window);
    sonic->render(*window);
    tails->render(*window);
    knuckles->render(*window);
    window->display();
}

void Game::switchPlayer() {
    PlayerCharacter* prev = activePlayer;
    activeIndex = (activeIndex + 1) % 3;
    if (activeIndex == 0) activePlayer = sonic;
    else if (activeIndex == 1) activePlayer = tails;
    else                        activePlayer = knuckles;
    repositionAfterSwitch(prev);
    std::cout << "Switched active player\n";
}

void Game::repositionAfterSwitch(PlayerCharacter* prev) {
    PlayerCharacter* third;
    if (activePlayer != sonic && prev != sonic)    third = sonic;
    else if (activePlayer != tails && prev != tails)    third = tails;
    else                                                    third = knuckles;

    float y = activePlayer->getPosition().y;
    if (lastDirRight) {
        prev->setPosition(activePlayer->getPosition().x - formationSpacing, y);
        third->setPosition(activePlayer->getPosition().x - 2 * formationSpacing, y);
    }
    else {
        prev->setPosition(activePlayer->getPosition().x + formationSpacing, y);
        third->setPosition(activePlayer->getPosition().x + 2 * formationSpacing, y);
    }
}
