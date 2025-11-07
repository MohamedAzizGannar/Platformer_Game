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

const float PLAYER_SPEED = 300.f;
const float JUMP_SPEED = 450.f;
const float JUMP_COOLDOWN = .3f;
const float GRAVITY = 12.f;
class GameManager {
private:
  sf::ContextSettings contextSettings;
  sf::Clock clock;
  sf::RenderWindow window;

  float currentTime =0.f;
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
    inputSystem.trackKey(sf::Keyboard::Key::Space);

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
      movementSystem.update(*character, dt);
      bool isGrounded = false;
      for (auto &platform : platforms) {
        if (platform->getIsGround()) {
          auto info = collisionSystem.checkCollisions(
              character->getCollider(), character->getTransform(),
              platform->getCollider(), platform->getTransform());
          if (info.isCollided) {
            Velocity zeroes(0.f, 0.f);
            collisionSystem.ResolveCollisions(
                character->getTransform(), character->getVelocity(),
                character->getCollider(), platform->getTransform(), zeroes,
                platform->getCollider(), info);
            if (collisionSystem.isGrounded(
                    character->getTransform(), character->getCollider(),
                    platform->getTransform(), platform->getCollider()))
              isGrounded = true;
          }
          character->setGrounded(isGrounded);
          if (!character->getIsGrounded())
            movementSystem.addGravity(*character, GRAVITY);
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

      if (inputSystem.jumpPressed() && (currentTime - player->getLastJumpTime() >JUMP_COOLDOWN ) && player->getIsGrounded()) {
        player->getVelocity().y -= JUMP_SPEED;
        player->setLastJumpTime(currentTime);
      }
    }
  }
  void run() {
    while (window.isOpen()) {
      float dt = clock.restart().asSeconds();
      currentTime += dt;
      handleEvents();
      handleInputs();
      update(dt);
      render();
    }
  }
};
