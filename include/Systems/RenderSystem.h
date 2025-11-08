#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <SFML/Graphics.hpp>
class RenderSystem {
public:
  void render(Entity &entity, sf::RenderWindow &window) {
    if (!entity.transform)
      return;
    if (entity.sprite) {
      auto &sprite = entity.sprite.value();
      auto &transform = entity.transform.value();

      sprite.sprite.setPosition({transform.x, transform.y});
      window.draw(sprite.sprite);
    } else if (entity.shape) {
      entity.shape->shape.setPosition(
          {entity.transform.value().x, entity.transform.value().y});
      window.draw(entity.shape->shape);
    }
  }
  void debugRender(Entity &entity, sf::RenderWindow &window) {
    if(!entity.collider || !entity.transform) return;
    auto &coll = entity.collider.value();
    auto bounds = coll.getBounds(entity.transform.value());
    sf::RectangleShape boundsShape({bounds.size.x, bounds.size.y});
    boundsShape.setPosition({bounds.position.x, bounds.position.y});
    boundsShape.setOutlineColor(sf::Color::Green);
    boundsShape.setFillColor(sf::Color::Transparent);
    window.draw(boundsShape);
  }
};
