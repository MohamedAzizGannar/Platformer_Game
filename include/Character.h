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

public:
  Character(float x, float y, 
            float maxHealth)
      : transform(x, y),collider(10.f,50.f), shape(sf::Color::Red), health(maxHealth) {
  }
  Transform& getTransform() { return transform; }
  Velocity& getVelocity() { return velocity; }
  State& getState() { return playerState; }
  Shape& getShape(){return shape; }
  Collider& getCollider(){return collider;}

  float getCurrentHealth() { return health.current; }
  float getMaxHealth() { return health.max; }
  float getHealthPercentage() { return health.getPercentage(); }
	void takeDamage(float amount){ health.takeDamage(amount);}
	void heal(float amount){ health.heal(amount);}

  void draw(sf::RenderWindow &window) {
    shape.render(window);
  
  }
};
