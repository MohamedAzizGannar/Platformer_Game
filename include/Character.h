#pragma once
#include "Components.h"
#include <SFML/Graphics.hpp>
enum class State { RUNNING, IDLE, ATTACK, HURT };
class Character {
private:
  State playerState;
  Transform transform;
  Velocity velocity;
  Collider collider;
  Shape shape;
  Health health;

  bool isGrounded;
  float lastJumpTime;

public:
  Character(float x, float y, float width, float height, float maxHealth)
      : transform(x, y), collider(width, height), shape(sf::Color(21, 32, 132),width,height),
        health(maxHealth) {}
  Transform &getTransform() { return transform; }
  Velocity &getVelocity() { return velocity; }
  State &getState() { return playerState; }
  Shape &getShape() { return shape; }
  Collider &getCollider() { return collider; }
  bool getIsGrounded() { return isGrounded; }
  void setGrounded(bool state) { isGrounded = state; }
  void setLastJumpTime(float time) { lastJumpTime = time; }
  float getLastJumpTime() { return lastJumpTime; }
  float getCurrentHealth() { return health.current; }
  float getMaxHealth() { return health.max; }
  float getHealthPercentage() { return health.getPercentage(); }
  void takeDamage(float amount) { health.takeDamage(amount); }
  void heal(float amount) { health.heal(amount); }

  void draw(sf::RenderWindow &window) { shape.render(window); }
};
