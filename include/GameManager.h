#pragma once
#include <Character.h>
#include <CollisionSystem.h>
#include <InputSystem.h>
#include <MovementSystem.h>
#include <Platform.h>
#include <RenderSystem.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <vector>

const float PLAYER_SPEED = 500.f;
const float JUMP_SPEED = 750.f;
const float GRAVITY = 100.f;
class GameManager {
private:
  sf::ContextSettings contextSettings;
  sf::Clock clock;
  sf::RenderWindow window;

  std::vector<std::unique_ptr<Character>> characters;
  std::vector<std::unique_ptr<Platform>> platforms;
  CollisionSystem collisionSystem;
  InputSystem inputSystem;
  MovementSystem movementSystem;
  RenderSystem renderSystem;

public:
  GameManager(uint32_t WINDOW_WIDTH, uint32_t WINDOW_HEIGHT)
      : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Game") {
    inputSystem.trackKey(sf::Keyboard::Key::A);
    inputSystem.trackKey(sf::Keyboard::Key::D);
    inputSystem.trackKey(sf::Keyboard::Key::Left);
    inputSystem.trackKey(sf::Keyboard::Key::Right);
    inputSystem.trackKey(sf::Keyboard::Key::W);
    inputSystem.trackKey(sf::Keyboard::Key::Up);
    inputSystem.trackKey(sf::Keyboard::Key::Down);
    window.setFramerateLimit(60);
  }
  void addCharacter(std::unique_ptr<Character> character) {
    characters.push_back(std::move(character));
  }
  void addPlatform(std::unique_ptr<Platform> platform) {
    platforms.push_back(std::move(platform));
  }
  void update(float dt) {
    for (auto &character : characters) {
      movementSystem.addGravity(*character, GRAVITY,dt);
      movementSystem.update(*character, dt);
      for (auto &platform : platforms) {
        auto info = collisionSystem.checkCollisions(
            character->getCollider(), character->getTransform(),
            platform->getCollider(), platform->getTransform());
        if (info.isCollided) {
          printf("Collision Normal = %.2f %.2f Overlay = %.2f \n", info.normal.x, info.normal.y, info.overlay);
          Velocity zeroes(0.f, 0.f);
          collisionSystem.ResolveCollisions(
              character->getTransform(), character->getVelocity(),
              character->getCollider(), platform->getTransform(), zeroes,
              platform->getCollider(), info);
        }
      }

    }
  }
  void render() {
    window.clear(sf::Color::White);
    for (auto &character : characters) {
      renderSystem.renderCharacter(*character, window);
    }
    for (auto &platform : platforms) {
      renderSystem.renderPlatform(*platform, window);
    }
    window.display();
  }
  void handleEvents() {
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
  void handleInputs() {
    inputSystem.update();

    if (!characters.empty()) {
      auto &player = characters[0];

      float horizontalInput = inputSystem.getAxisX();
      player->getVelocity().x = horizontalInput * PLAYER_SPEED;

      float verticalInput = inputSystem.getAxisY();
      player->getVelocity().y = verticalInput * JUMP_SPEED;
    }
  }
  void run() {
    while (window.isOpen()) {
      float dt = clock.restart().asSeconds();
      dt = std::min(0.05f,dt);
      handleEvents();
      handleInputs();
      update(dt);
      render();
    }
  }
};
