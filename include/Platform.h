#pragma once

#include <Components.h>
#include <SFML/Graphics.hpp>

class Platform {
private:
  Transform transform;
  Shape shape;
  Collider collider;
  bool isGround;

public:
  Platform(float x, float y, float width, float height, bool isGround = false)
      : transform(x, y), shape(sf::Color(82,60,60), width, height),
        collider(width, height, true), isGround(isGround) {
    shape.shape.setOrigin({width / 2.f, height / 2.f});
    shape.shape.setOutlineColor(sf::Color::Black);
  }

  Transform &getTransform() { return transform; }
  Shape &getShape() { return shape; }
  Collider &getCollider() { return collider; }
  bool &getIsGround() { return isGround; }

  void draw(sf::RenderWindow &window) { shape.render(window); }
};
