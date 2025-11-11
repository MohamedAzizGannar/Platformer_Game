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
    if (!entity.collider || !entity.transform)
      return;

    auto &coll = entity.collider.value();
    auto bounds = coll.getBounds(entity.transform.value());
    sf::RectangleShape boundsShape({bounds.size.x, bounds.size.y});
    boundsShape.setPosition({bounds.position.x, bounds.position.y});
    boundsShape.setOutlineColor(sf::Color::Green);
    boundsShape.setOutlineThickness(3.f);
    boundsShape.setFillColor(sf::Color::Transparent);

    auto &transform = entity.transform.value();
    sf::CircleShape transformDot(5.f);
    transformDot.setPosition({transform.x, transform.y});
    transformDot.setOrigin({5.f, 5.f});
    transformDot.setOutlineColor(sf::Color::White);
    transformDot.setOutlineThickness(3.f);
    transformDot.setFillColor(sf::Color::Transparent);

    if (entity.sprite) {
      auto &s = entity.sprite->sprite;
      sf::FloatRect globalBounds =
          s.getGlobalBounds(); // accounts for origin, scale, etc.

      sf::RectangleShape spriteBounds(
          {globalBounds.size.x, globalBounds.size.y});
      spriteBounds.setPosition({globalBounds.position.x, globalBounds.position.y});
      spriteBounds.setOutlineColor(sf::Color::Blue);
      spriteBounds.setOutlineThickness(2.f);
      spriteBounds.setFillColor(sf::Color::Transparent);

      window.draw(spriteBounds);
    }
    window.draw(boundsShape);
    window.draw(transformDot);
  }
};
