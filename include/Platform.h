#pragma once

#include <Components.h>
#include <SFML/Graphics.hpp>

class Platform {
private:
  Transform transform;
  Shape shape;
  Collider collider;

public:
  Platform(float x, float y, float width, float height)
      : transform(x, y), shape(sf::Color::Green, width, height),
        collider(width, height, true) {
    shape.shape.setOrigin({width / 2.f, height / 2.f});
    shape.shape.setOutlineColor(sf::Color::Black);
  }

  Transform &getTransform() { return transform; }
  Shape &getShape() { return shape; }
  Collider &getCollider() { return collider; }

  void draw(sf::RenderWindow &window) {
    shape.render(window);
  }
};
