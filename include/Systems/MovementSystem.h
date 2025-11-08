#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <SFML/Graphics.hpp>
#include <Systems/InputSystem.h>
const float GRAVITY = 900.f;
class MovementSystem {
public:
  void update(Entity &entity, const InputSystem &input, float dt) {
    if (!entity.movement || !entity.velocity || !entity.transform)
      return;
    if (entity.collider->isStatic)
      return;
    auto &vel = entity.velocity;
    auto &transform = entity.transform;
    auto &mov = entity.movement;
    float inputX = input.getAxisX();
    if (inputX != 0.f) {
      vel->x += inputX * mov.value().acceleration * dt;
      vel->x = std::clamp(vel->x, -entity.movement->maxSpeed,
                          entity.movement->maxSpeed);

      vel->y = std::clamp(vel->y, -entity.movement->maxSpeed,
                          entity.movement->maxSpeed);
    } else {
      vel->x *= std::max(0.f, 1.f - mov.value().friction * dt);
    }
    transform->x += vel.value().x * dt;
    transform->y += vel.value().y * dt;
  }
  void applyGravity(Entity &entity, float dt) {
    if (entity.collider->isStatic)
      return;
    entity.velocity->y += GRAVITY * dt;
  }
};
