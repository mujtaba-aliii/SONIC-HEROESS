#include "Game.h"
#include <SFML/Audio.hpp>

#include <iostream>

Game::Game(int screenX, int screenY)
    : window(new sf::RenderWindow(sf::VideoMode(screenX, screenY), "Sonic the Hedgehog - OOP"))
    , view(sf::FloatRect(0, 0, (float)screenX, (float)screenY))
    , level(14, 110, 64)
    , activeIndex(0)
{
    if (!backgroundMusic.openFromFile("Data/labrynth1.wav"))
        std::cerr << "Failed to load music\n";
    else {
        backgroundMusic.setVolume(30.f);
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    window->setView(view);
    level.initGridFromArray();
    level.ringsPlacement();
    level.spawnCrabs();
    
	ringCount = level.getRingsCount();

    rings = new collectibles * [ringCount];  // dynamically allocate array of pointers
    float groundY = (level.getHeight() - 1) * level.getCellSize();
    int i = 0;
    while (i < ringCount) {
        float x = level.getRingX(i);
        float y = level.getRingY(i);

        rings[i] = new collectibles(x, y);
        rings[i]->setTexture();
        rings[i]->setScale(2.5f, 2.5f);

        // Position using grid Y minus the height of the sprite to align bottom
        rings[i]->setPosition(x, y - rings[i]->getSpriteHeight());

        i = i + 1;
    }

    crabCount = level.getCrabCount();
    crab = new Crabmeat * [crabCount];  // dynamically allocate array of pointers
    groundY = (level.getHeight() - 1) * level.getCellSize();
    i = 0;
    while (i < crabCount) {
        float x = level.getCrabX(i);
        float y = level.getCrabY(i);

        crab[i] = new Crabmeat(x, y -35);
        crab[i]->setScale(2.5f, 2.5f);
        crab[i]->setPosition(x, groundY - crab[i]->getSpriteHeight());
        i = i + 1;
    }


    const float startX = 100.f, spacing = 70.f;
    sonic = new Sonic(0, 0);
    tails = new Tails(0, 0);
    knuckles = new Knuckles(0, 0);
	
    motobug = new MotoBug(0, 0);
    float motoX = screenX - motobug->getSpriteWidth();
   groundY = (level.getHeight() - 1) * level.getCellSize();
    float motoY = groundY - motobug->getSpriteHeight();
    motobug = new MotoBug(motoX-400, motoY - 100);

    buzzbomber = new BuzzBomber(0, 0);
    float buzzX = screenX - buzzbomber->getSpriteWidth();
    groundY = (level.getHeight() - 1) * level.getCellSize();
    /*float buzzY = groundY + buzzbomber->getSpriteHeight();*/
    buzzbomber = new BuzzBomber(buzzX - 400, 150);

    batbrain = new BatBrain(0, 0);
    float batX = screenX - batbrain->getSpriteWidth();
    groundY = (level.getHeight() - 1) * level.getCellSize();
    batbrain = new BatBrain(buzzX - 400, 50);

    sonic->setScale(2.5f, 2.5f);
    tails->setScale(2.5f, 2.5f);
    knuckles->setScale(2.5f, 2.5f);
	batbrain->setScale(2.5f, 2.5f);
   /* crab->setScale(2.5f, 2.5f);*/
	motobug->setScale(2.5f, 2.5f);
	buzzbomber->setScale(2.5f, 2.5f);
     groundY = (level.getHeight() - 1) * level.getCellSize();
    knuckles->setPosition(startX, groundY - knuckles->getSpriteHeight());
    tails->setPosition(startX + spacing, groundY - tails->getSpriteHeight());
    sonic->setPosition(startX + 2 * spacing, groundY - sonic->getSpriteHeight());
    /*crab->setPosition(startX + 4 * spacing, groundY - crab->getSpriteHeight());*/
    motobug->setPosition(startX + 3 * spacing, groundY - motobug->getSpriteHeight());
	buzzbomber->setPosition(startX + 5 * spacing, groundY - buzzbomber->getSpriteHeight());
    batbrain->setPosition(startX + 6 * spacing, groundY - buzzbomber->getSpriteHeight());
    activePlayer = sonic;
    sonic->toggleDebug();
   
    std::cout << "Game initialized. Use arrow keys to move, Up to jump." << std::endl;
}

Game::~Game() {
    delete sonic;
    delete tails;
    delete knuckles;
    delete crab;
    delete motobug;
	delete buzzbomber;
    delete batbrain;
    delete rings;
    delete window;
    
}

void Game::run() {
    sf::Clock clock;
    while (window->isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event ev;
    while (window->pollEvent(ev)) {
        if (ev.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window->close();
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Z)
            switchPlayer();
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::F1)
            activePlayer->toggleDebug();
        activePlayer->handleInput(ev);
    }
}

void Game::update(float dt) {
    float safeDt = dt > 0.0001f ? dt : 0.0001f;
    sonic->update(safeDt, level);
    tails->update(safeDt, level);
    knuckles->update(safeDt, level);
    int i = 0;
    while (i < crabCount){
    crab[i]->update(safeDt, level);
    i += 1;
    }
    i = 0;
    while (i < ringCount) {
        rings[i]->update(safeDt,activePlayer);
        i += 1;
    }
    motobug->update(safeDt, level);
	buzzbomber->update(safeDt, level);
	batbrain->update(safeDt, level);
    sf::Vector2f pos = activePlayer->getPosition();
    float halfW = view.getSize().x / 2.f;
    pos.x = std::max(halfW, std::min(pos.x, level.getWidth() * level.getCellSize() - halfW));
    view.setCenter(pos.x, view.getCenter().y);
    window->setView(view);
}

void Game::render() {
    window->clear();
    level.render(*window);
    sonic->render(*window);
    tails->render(*window);
    knuckles->render(*window);
	int i = 0;
    while (i < crabCount) {
        crab[i]->render(*window);
        i++;
    }
    i = 0;
    while (i < ringCount) {
        rings[i]->render(*window);
        i++;
    }

    motobug->render(*window);
	buzzbomber->render(*window);
    batbrain->render(*window);
    window->display();
}

void Game::switchPlayer() {
    activeIndex = (activeIndex + 1) % 3;
    if (activeIndex == 0)        activePlayer = sonic;
    else if (activeIndex == 1)   activePlayer = tails;
    else                       activePlayer = knuckles;
    std::cout << "Switched active player" << std::endl;
}
