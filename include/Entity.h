#pragma once


#include <SFML/Graphics.hpp>

#include <Components/Components.h>

struct Entity{
  int id;
  std::optional<Transform> transform;
  std::optional<Velocity> velocity;
  std::optional<JumpComponent> jump;
  std::optional<Movement> movement;
  std::optional<Health> health;
  std::optional<Collider> collider;
  std::optional<Shape> shape;
};
