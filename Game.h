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
#include "Batbrain.h"
#include "Collectibles.h"
class Game {
public:
    Game(int screenX, int screenY);
    ~Game();
    void run();
private:
    void processEvents();
    void update(float dt);
    void render();
    void switchPlayer();
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::RenderWindow* window;
    sf::View view;
    Level level;
    collectibles** rings;
    Sonic* sonic;
    Tails* tails;
    Knuckles* knuckles;
    PlayerCharacter* activePlayer;
	EnemyCharacter* enemy;
    Crabmeat** crab;
	MotoBug* motobug;
	BuzzBomber* buzzbomber;
	BatBrain* batbrain;
    int activeIndex;
    int crabCount;
    int ringCount;
    sf::Music backgroundMusic;
    
    
    
};
