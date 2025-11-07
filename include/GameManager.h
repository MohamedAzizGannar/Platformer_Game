#pragma once
#include <Character.h>
#include <MovementSystem.h>
#include <RenderSystem.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <vector>

class GameManager {
private:
  sf::ContextSettings contextSettings;
  sf::Clock clock;
  sf::RenderWindow window;
  std::vector<Character> characters;
  MovementSystem movementSystem;
  RenderSystem renderSystem;

public:
  GameManager(uint32_t WINDOW_WIDTH, uint32_t WINDOW_HEIGHT)
      : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Game") {
    window.setFramerateLimit(60);
  }
  void addCharacter(Character &character) { characters.push_back(character); }
  void update(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      for (auto &character : characters) {
        auto vel = character.getVelocity(); 
        character.getVelocity().setVelocity(vel.x + 50.f, vel.y + 50.f);
      }
    }
    for (auto &character : characters) {
      movementSystem.update(character, dt);
    }
  }
  void render() {
    window.clear(sf::Color::White);
    for (auto &character : characters) {
      renderSystem.renderCharacter(character, window);
    }
    window.display();
  }
  void handleInputs() {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
          window.close();
        }
      }
    }
  }
  void run() {
    while (window.isOpen()) {
      float dt = clock.restart().asSeconds();
      handleInputs();
      update(dt);
      render();
    }
  }
};
