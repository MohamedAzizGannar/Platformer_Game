#pragma once
#include <SFML/Graphics.hpp>
#include <Character.h>
#include <Components.h>

class MovementSystem{
public:
  void update(Character& character, float dt){
    auto& transform = character.getTransform();
    auto& velocity = character.getVelocity();
    transform.translate(velocity.toVector().x * dt, velocity.toVector().y * dt);
  }
  void addGravity(Character& character,float gravity, float dt){
    character.getTransform().translate(0,gravity*dt);

  }
  sf::Vector2f predictPosition(Character& character, float dt){
    auto currentPos = character.getTransform().toVector();
    auto currentVelocity = character.getVelocity().toVector();

    auto predictedPos = currentPos + currentVelocity * dt;
    return predictedPos;
  }
};

