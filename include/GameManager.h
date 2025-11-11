#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <Systems/AnimationLoader.h>
#include <Systems/AnimationSystems.h>
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
  AnimationSystem animationSystem;
  std::unordered_map<std::string, sf::Texture> textures;
  std::unordered_map<std::string, EntityAnimConfig> animConfigs;

public:
  GameManager(uint32_t WINDOW_WIDTH, uint32_t WINDOW_HEIGHT)
      : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Game") {
    inputSystem.trackKey(sf::Keyboard::Key::A);
    inputSystem.trackKey(sf::Keyboard::Key::D);
    inputSystem.trackKey(sf::Keyboard::Key::Left);
    inputSystem.trackKey(sf::Keyboard::Key::Right);
    inputSystem.trackKey(sf::Keyboard::Key::Space);
    inputSystem.trackKey(sf::Keyboard::Key::LShift);
    inputSystem.trackKey(sf::Keyboard::Key::W);
    inputSystem.trackKey(sf::Keyboard::Key::E);

    window.setFramerateLimit(60);
    LoadRessources();
  }
  void addEntity(Entity &entity) {
    entity.id = entities.size();
    entities.push_back(entity);
  }
  void LoadRessources() {
    animConfigs =
        AnimationLoader::loadALlAnimsFromFolder("../assets/Animations");
    for (const auto &[name, config] : animConfigs) {
      sf::Texture texture;
      if (texture.loadFromFile(config.texturePath)) {
        textures[name] = std::move(texture);
        std::cout << "✓ Loaded texture: " << name << std::endl;
      } else {
        std::cerr << "✗ Failed to load: " << config.texturePath << std::endl;
      }
    }
  }
  sf::Texture *getTexture(const std::string &name) {
    auto it = textures.find(name);
    return (it != textures.end()) ? &it->second : nullptr;
  }
  EntityAnimConfig *getAnimConfig(const std::string &name) { 
    auto it = animConfigs.find(name);
    return (it != animConfigs.end()) ? &it->second : nullptr;
  }
  void updateEntityAnimation(Entity &entity, const InputSystem &input) {
    if (!entity.animation || !entity.tag)
      return;

    if (entity.tag->tag != EntityLayer::Player)
      return;

    auto &anim = entity.animation.value();
    float axisX = input.getAxisX();
    sf::Vector2f currScale = entity.sprite->sprite.getScale();
    if (axisX != 0.f && entity.sprite) {
      float flip = (axisX < 0 ? -1.f : 1.f);
      entity.sprite->sprite.setScale(
          {flip * entity.sprite->scale, entity.sprite->scale});
    }
    if (input.isKeyPressed(sf::Keyboard::Key::E)) {
      AnimationSystem::changeAnimation(anim, "shoot");
    }
    if (input.isKeyPressed(sf::Keyboard::Key::W)) {
      AnimationSystem::changeAnimation(anim, "attack");
    } else if (axisX != 0.f) {
      AnimationSystem::changeAnimation(anim, "walk");
    } else {
      AnimationSystem::changeAnimation(anim, "idle");
    }
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
        updateEntityAnimation(entity, inputSystem);
      }
      for (auto &other : entities) {
        if (other.id == entity.id)
          continue;
        CollisionInfo info = collisionSystem.checkCollisions(entity, other);
        if (info.isCollided) {
          collisionSystem.ResolveCollisions(entity, other, info);
        }
      }
      if (entity.shape) {
        entity.shape->shape.setPosition(
            {entity.transform.value().x, entity.transform.value().y});
      }
      if (entity.sprite) {
        entity.sprite->sprite.setPosition(
            {entity.transform.value().x, entity.transform.value().y});
      }
      animationSystem.update(entity, dt);
    }
    groundCheckSystem.update(entities);
  }
  void render() {
    window.clear(sf::Color::White);
    for (auto &entity : entities) {
      renderSystem.render(entity, window);
      renderSystem.debugRender(entity, window);
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
