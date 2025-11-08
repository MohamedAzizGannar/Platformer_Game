#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <Systems/CollisionSystem.h>
#include <Systems/DashSystem.h>
#include <Systems/GroundCheckSystem.h>
#include <Systems/InputSystem.h>
#include <Systems/JumpSystem.h>
#include <Systems/MovementSystem.h>
#include <Systems/RenderSystem.h>

#include <Entity.h>
#include <vector>
class GameManager {
private:
  sf::ContextSettings contextSettings;
  sf::Clock clock;
  sf::RenderWindow window;

  std::vector<Entity> entities;

  float currentTime = 0.f;
  CollisionSystem collisionSystem;
  InputSystem inputSystem;
  MovementSystem movementSystem;
  RenderSystem renderSystem;
  GroundCheckSystem groundCheckSystem;
  JumpSystem jumpSystem;
  DashSystem dashSystem;

public:
  GameManager(uint32_t WINDOW_WIDTH, uint32_t WINDOW_HEIGHT)
      : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Game") {
    inputSystem.trackKey(sf::Keyboard::Key::A);
    inputSystem.trackKey(sf::Keyboard::Key::D);
    inputSystem.trackKey(sf::Keyboard::Key::Left);
    inputSystem.trackKey(sf::Keyboard::Key::Right);
    inputSystem.trackKey(sf::Keyboard::Key::Space);
    inputSystem.trackKey(sf::Keyboard::Key::LShift);

    window.setFramerateLimit(60);
  }
  void addEntity(Entity &entity) {
    entity.id = entities.size();
    entities.push_back(entity);
  }
  void update(float dt) {
    inputSystem.update();
    for (auto &entity : entities) {
      bool isStatic = entity.collider && entity.collider->isStatic;
      if (!isStatic) {
        jumpSystem.update(entity, inputSystem, dt);
        dashSystem.update(entity, inputSystem, dt);
        if (!entity.dash.value().isDashing) {
          movementSystem.update(entity, inputSystem, dt);
          movementSystem.applyGravity(entity, dt);
        }
      }
      for (auto &other : entities) {
        if (other.id == entity.id)
          continue;
        CollisionInfo info = collisionSystem.checkCollisions(entity, other);
        if (info.isCollided) {
          collisionSystem.ResolveCollisions(entity, other, info);
        }
      }
      entity.shape->shape.setPosition(
          {entity.transform.value().x, entity.transform.value().y});
    }
    groundCheckSystem.update(entities);
  }
  void render() {
    window.clear(sf::Color::White);
    for (auto &entity : entities) {
      renderSystem.render(entity, window);
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
  void run() {
    while (window.isOpen()) {
      handleEvents();
      float dt = clock.restart().asSeconds();
      update(dt);
      render();
    }
  }
};
