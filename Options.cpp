#include "Options.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

void Button::run(sf::RenderWindow& window) {
    sf::Music music;
    if (!music.openFromFile("Data/labrynth1.wav")) return;
    music.setLoop(true);
    music.play();

    sf::Font font;
    font.loadFromFile("Data/arial.ttf");

    Button increaseButton(300.f, 350.f, " +");
    Button decreaseButton(550.f, 350.f, " -");

    increaseButton.setFont(font);
    decreaseButton.setFont(font);

    bool optionsActive = true;

    while (optionsActive && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                optionsActive = false;

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (increaseButton.isClicked(mousePos)) {
                    float vol = music.getVolume();
                    if (vol < 100) music.setVolume(vol + 10);
                }

                if (decreaseButton.isClicked(mousePos)) {
                    float vol = music.getVolume();
                    if (vol > 0) music.setVolume(vol - 10);
                }
            }
        }

        window.clear();
        increaseButton.draw(window);
        decreaseButton.draw(window);
        window.display();
    }
}
