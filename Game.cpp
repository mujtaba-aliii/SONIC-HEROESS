#include "Game.h"
#include <iostream>
#include <algorithm>
#include <SFML/Audio.hpp>

LevelConfig Game::getLevelConfig(int n) {
    switch (n) {
    case 1: return { 1,200,14,64,"Data/bg1.png","Data/labrynth1.wav", 0.8f,20.f,15.f,1.0f,0.8f };
    case 2: return { 2,250,14,64,"Data/bg2.png","Data/labrynth1.wav",    0.8f,20.f,15.f,1.5f,0.5f };
    case 3: return { 3,300,14,64,"Data/bg3.png","Data/labrynth1.wav",  0.4f,15.f,10.f,0.8f,0.6f };
    case 4: return { 4,20,14,64,"Data/bg1.png","Data/labyrnth1.wav",  0.6f,15.f,12.f,1.0f,0.7f };
    default:return { 1,200,14,64,"","",0.8f,20.f,15.f,1.0f,0.8f };
    }
}

Game::Game(int screenX, int screenY, int levelNumber)
    : window(new sf::RenderWindow(sf::VideoMode(screenX, screenY), "Sonic - OOP"))
    , view(sf::FloatRect(0, 0, (float)screenX, (float)screenY))
    , activeIndex(0), lastDirRight(true), currentCap(startSpeed)
{
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    window->setView(view);

    LevelConfig cfg = getLevelConfig(levelNumber);
    level = new Level(cfg);

    // Play level music
    if (!backgroundMusic.openFromFile(cfg.musicFile))
        std::cerr << "Failed to load music\n";
    else {
        backgroundMusic.setVolume(30.f);
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }
    if (!font.loadFromFile("Data/arial.ttf")) {
        std::cout << "ERROR" << std::endl;
    }
    int score = 0;
    int ls = 3;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(10, 10);

    lsText.setFont(font);
    lsText.setCharacterSize(40);
    lsText.setFillColor(sf::Color::White);
    lsText.setPosition(120, 50);

    ringText.setFont(font);
    ringText.setCharacterSize(40);
    ringText.setFillColor(sf::Color::White);
    ringText.setPosition(120, 100);

    level->ringsPlacement(); level->livesPlacement(); level->boostPlacement();
    level->spawnCrabs();     crabCount = level->getCrabCount();
    level->spawnSpikes();    level->spawnPlatforms();
    level->spawnBugs();      level->spawnBats();
    level->spawnBUZZ();

    // collectibles
    int rc=level->getRingsCount(), lc=level->getLivesCount(), bc=level->getBoostCount();
    rings  = new collectibles*[rc];
    lives  = new collectibles*[lc];
    boost  = new collectibles*[bc];

    for(int i=0;i<rc;++i){
        float x=level->getRingX(i), y=level->getRingY(i);
        rings[i]=new collectibles(x,y,"ring"); rings[i]->setScale(2.5f,2.5f);
    }
    for(int i=0;i<lc;++i){
        float x=level->getLivesX(i), y=level->getLivesY(i);
        lives[i]=new collectibles(x,y,"lives"); lives[i]->setScale(2.5f,2.5f);
    }
    for(int i=0;i<bc;++i){
        float x=level->getBoostX(i), y=level->getBoostY(i);
        boost[i]=new collectibles(x,y,"boost"); boost[i]->setScale(2.5f,2.5f);
    }

    // Crabs
    crab = new Crabmeat*[crabCount];
    for(int i=0;i<crabCount;++i){
        float x=level->getCrabX(i), y=level->getCrabY(i)-35;
        crab[i]=new Crabmeat(x,y); crab[i]->setScale(2.5f,2.5f);
    }

    //bugs
    bugCount = level->getBugsCount();
    bug = new MotoBug * [bugCount]; 
    float groundY = (level->getHeight() - 1) * level->getCellSize();
    int i = 0;
    while (i < bugCount) {
        float x = level->getbugX(i);
        float y = level->getbugY(i);

        bug[i] = new MotoBug(x, y - 35);
        bug[i]->setTexture();
        bug[i]->setScale(2.5f, 2.5f);
        bug[i]->setPosition(x, groundY - bug[i]->getSpriteHeight());
        i = i + 1;
    }

    //Bats
    batCount = level->getBatsCount();
    bat = new BatBrain * [batCount];
    groundY = (level->getHeight() - 1) * level->getCellSize();
    i = 0;
    while (i < batCount) {
        float x = level->getBatsX(i);
        float y = level->getBatsY(i);


        bat[i] = new BatBrain(x, y - 35);
        bat[i]->setScale(2.5f, 2.5f);
        bat[i]->setPosition(x, groundY - bat[i]->getSpriteHeight());
        i = i + 1;
    }

    //BUZZ
    buzzCount = level->getBuzzCount();
    buzzbomber = new BuzzBomber * [buzzCount];
    groundY = (level->getHeight() - 1) * level->getCellSize();
    i = 0;
    while (i < buzzCount) {
        float x = level->getBuzzX(i);
        float y = level->getBuzzY(i);

        buzzbomber[i] = new BuzzBomber(x, y - 35);
        buzzbomber[i]->setScale(2.5f, 2.5f);
        buzzbomber[i]->setPosition(x, y - buzzbomber[i]->getSpriteHeight());
        i = i + 1;
    }

    //Boss
    level->spawnBOSS();
    float x = level->getBossX();
    boss = new Boss(35, 35);
    boss->setScale(2.5f, 2.5f);
    boss->setTexture();
    bool bossActive = true;
    boss->setPosition(35, groundY - boss->getSpriteHeight());
    boss->setScale(2.5f, 2.5f);
    // Spikes
    int sc=level->getSpikesCount();
    spikes=new obstacles*[sc];
    for(int i=0;i<sc;++i){
        float x=level->getSpikesX(i), y=level->getSpikesY(i);
        spikes[i]=new obstacles(x,y,"spike",cfg.cellSize);
        spikes[i]->setScale(2.5f,2.5f);
    }

    // Platforms
    int pc=level->getPlatformsCount();
    platforms=new obstacles*[pc];
    for(int i=0;i<pc;++i){
        float x=level->getPlatformsX(i), y=level->getPlatformsY(i);
        platforms[i]=new obstacles(x,y,"platform",cfg.cellSize);
    }

    const float startX = 100.f, spacing = 70.f;
    groundY = (cfg.height - 1) * cfg.cellSize;

    // Sonic
    sonic = new Sonic(
        0,           
        0,                      
        cfg.gravity,                  
        cfg.terminalVel,              
        cfg.maxSpeed,                
        cfg.acceleration,             
        cfg.deceleration              
    );

    // Tails
    tails = new Tails(
        0,
        0,
        cfg.gravity,
        cfg.terminalVel,
        cfg.maxSpeed,
        cfg.acceleration,
        cfg.deceleration
    );

    // Knuckles
    knuckles = new Knuckles(
        0,
        0,
        cfg.gravity,
        cfg.terminalVel,
        cfg.maxSpeed,
        cfg.acceleration,
        cfg.deceleration
    );
    knuckles->setPosition(100.f, groundY - knuckles->getSpriteHeight());
    tails->setPosition(170.f, groundY - tails->getSpriteHeight());
    sonic->setPosition(240.f, groundY - sonic->getSpriteHeight());
    sonic->setScale(2.5f, 2.5f);
    tails->setScale(2.5f, 2.5f);
    knuckles->setScale(2.5f, 2.5f);
    activePlayer = sonic;
    sonic->toggleDebug();

    std::cout<<"Game initialized. Use arrows to move, Up to jump.\n";
}
Game::~Game() {
    delete sonic;
    delete tails;
    delete knuckles;
    delete boss;
    if (crab) {
        for (int i = 0; i < crabCount; ++i) {
            delete crab[i];
        }
        delete[] crab;
    }

    if (rings) {
        for (int i = 0; i < level->getRingsCount(); ++i) {
            delete rings[i];
        }
        delete[] rings;
    }

    if (lives) {
        for (int i = 0; i < level->getLivesCount(); ++i) {
            delete lives[i];
        }
        delete[] lives;
    }

    if (boost) {
        for (int i = 0; i < level->getBoostCount(); ++i) {
            delete boost[i];
        }
        delete[] boost;
    }

    if (spikes) {
        for (int i = 0; i < level->getSpikesCount(); ++i) {
            delete spikes[i];
        }
        delete[] spikes;
    }

    if (platforms) {
        for (int i = 0; i < level->getPlatformsCount(); ++i) {
            delete platforms[i];
        }
        delete[] platforms;
    }
    delete bug;
    delete buzzbomber;
    delete bat;
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
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
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
    int levelWidth = level->getWidth() * level->getCellSize();

    if (activePlayer->isMoving()) {
        float targetCap = (activePlayer == sonic ? sonicTarget : followTarget);
        float rampRate = (targetCap - startSpeed) / rampDuration;
        currentCap = std::min(targetCap, currentCap + rampRate * safeDt);
    }
    else {
        currentCap = startSpeed;
    }

    if (activePlayer->getHP() <= 0) {
        activePlayer->setHp(3);
        window->close();
        return;
    }

    sonic->setMaxSpeed(currentCap);
    tails->setMaxSpeed(currentCap);
    knuckles->setMaxSpeed(currentCap);

    activePlayer->update(safeDt, *level, windowWidth, windowHeight);

    if (activePlayer->getPositionY() > 1000.f) {
        sf::Clock clock;
        while (clock.getElapsedTime().asSeconds() < 3.f) {
            window->clear(); 
            window->display(); 
        }

        window->close(); 
        return;
    }

    for (int i = 0; i < crabCount; ++i) {
        crab[i]->update(safeDt, *level, activePlayer);
    }

    for (int i = 0;i < bugCount;i++) {
        bug[i]->update(safeDt, *level, activePlayer);
    }

    for (int i = 0;i < batCount;i++) {
        bat[i]->update(safeDt, *level, activePlayer);
    }

    for (int i = 0;i < buzzCount;i++) {
        buzzbomber[i]->update(safeDt, *level, activePlayer, window);
    }

    boss->update(safeDt, *level, activePlayer);

    for (int i = 0; i < level->getRingsCount(); ++i) {
        rings[i]->update(safeDt, activePlayer);
    }

    lsText.setString(std::string("Lives: ") + std::to_string(activePlayer->getHP()));
    int totalRings = 0;
    for (int i = 0; i < level->getRingsCount(); ++i) {
        totalRings += rings[i]->getrings();
    }

    ringText.setString(std::string("Rings: ") + std::to_string(totalRings));


    for (int i = 0; i < level->getLivesCount(); ++i) {
        lives[i]->update(safeDt, activePlayer);
    }

    for (int i = 0; i < level->getBoostCount(); ++i) {
        boost[i]->update(safeDt, activePlayer);
    }

    for (int i = 0; i < level->getSpikesCount(); ++i) {
        spikes[i]->update(safeDt, activePlayer);
    }

    sf::Vector2f activePos = activePlayer->getPosition();
    float leaderVx = activePlayer->getVelocityX();

    constexpr float faceThresh = 0.1f;
    if (leaderVx > faceThresh) lastDirRight = true;
    if (leaderVx < -faceThresh) lastDirRight = false;

    constexpr float idleSpacing = 70.f;
    constexpr float movingSpacing = 100.f;
    float spacing = (std::abs(leaderVx) > faceThresh ? movingSpacing : idleSpacing);
    float doubleSpacing = spacing * 2.f;

    PlayerCharacter* f1;
    PlayerCharacter* f2;
    if (activePlayer == sonic) {
        f1 = tails;
        f2 = knuckles;
    }
    else if (activePlayer == tails) {
        f1 = knuckles;
        f2 = sonic;
    }
    else {
        f1 = sonic;
        f2 = tails;
    }

    if (tails->isGlidePhaseActive()) {
        sf::Vector2f tailPos = tails->getPosition();
        float tx = tailPos.x;
        float ty = tailPos.y;

        float dir = lastDirRight ? 1.f : -1.f;
        sonic->setPosition(tx - dir * 49.f, ty - 42.f);
        sonic->forceHang(lastDirRight);

        sf::Vector2f sonicPos = sonic->getPosition();
        float sx = sonicPos.x;
        float sy = sonicPos.y;

        knuckles->setPosition(sx - dir * 49.f, sy + 44.f);
        knuckles->forceHang(lastDirRight);
    }
    else {
        if (lastDirRight) {
            f1->followTarget(activePos.x - spacing, activePos.y, leaderVx, safeDt, *level);
            f2->followTarget(activePos.x - doubleSpacing, activePos.y, leaderVx, safeDt, *level);
        }
        else {
            f1->followTarget(activePos.x + spacing, activePos.y, leaderVx, safeDt, *level);
            f2->followTarget(activePos.x + doubleSpacing, activePos.y, leaderVx, safeDt, *level);
        }
    }

    f1->clampPosition(windowWidth, windowHeight, levelWidth);
    f2->clampPosition(windowWidth, windowHeight, levelWidth);

    sf::Vector2f camPos = activePos;
    float halfW = view.getSize().x / 2.f;
    float levelRight = levelWidth - halfW;
    camPos.x = std::max(halfW, std::min(camPos.x, levelRight));
    view.setCenter(camPos.x, view.getCenter().y);
    window->setView(view);
}


void Game::render() {
    window->clear();
    level->render(*window);
    sonic->render(*window);
    tails->render(*window);
    knuckles->render(*window);
    
    for (int i = 0; i < 4; ++i) {
        window->draw(menu[i]);
    }

    window->draw(scoreText);
    window->draw(lsText);
    window->draw(ringText);

    for (int i = 0; i < crabCount; ++i) {
        crab[i]->render(*window);
    }

    int i = 0;
    while (i < bugCount) {
        bug[i]->render(*window);
        i++;
    }
    i = 0;
    while (i < batCount) {
        bat[i]->render(*window);
        i++;
    }
    i = 0;
    while (i < buzzCount) {
        buzzbomber[i]->render(*window);
        i++;
    }

    boss->render(window);

    for (int i = 0; i < level->getRingsCount(); ++i) {
        rings[i]->render(*window);
    }

    for (int i = 0; i < level->getLivesCount(); ++i) {
        lives[i]->render(*window);
    }

    for (int i = 0; i < level->getBoostCount(); ++i) {
        boost[i]->render(*window);
    }

    for (int i = 0;i < level->getSpikesCount();++i) {
        spikes[i]->render(*window);
    }

    for (int i = 0;i < level->getPlatformsCount();++i) {
        platforms[i]->render(*window);
    }

    for (int i = 0;i < level->getBugsCount();++i) {
        bug[i]->render(*window);
    }

    for (int i = 0;i < level->getBatsCount();++i) {
        bat[i]->render(*window);
    }

    for (int i = 0;i < level->getBuzzCount();++i) {
        buzzbomber[i]->render(*window);
    }
    window->draw(scoreText);
    window->draw(lsText);
    window->draw(ringText);
    window->display();
}

void Game::switchPlayer() {
    PlayerCharacter* prev = activePlayer;
    activeIndex = (activeIndex + 1) % 3;
    if (activeIndex == 0) activePlayer = sonic;
    else if (activeIndex == 1) activePlayer = tails;
    else activePlayer = knuckles;
    repositionAfterSwitch(prev);
    std::cout << "Switched active player\n";
}

void Game::repositionAfterSwitch(PlayerCharacter* prev) {
    PlayerCharacter* third;
    if (activePlayer != sonic && prev != sonic) third = sonic;
    else if (activePlayer != tails && prev != tails) third = tails;
    else third = knuckles;

    float y = activePlayer->getPosition().y;
    if (lastDirRight) {
        prev->setPosition(activePlayer->getPosition().x - 70.f, y);
        third->setPosition(activePlayer->getPosition().x - 2 * 70.f, y);
    }
    else {
        prev->setPosition(activePlayer->getPosition().x + 70.f, y);
        third->setPosition(activePlayer->getPosition().x + 2 * 70.f, y);
    }
}