#pragma once
#include <SFML/Graphics.hpp>
#include <Components/Components.h>
#include <Entity.h>
class RenderSystem {
public:
  void render(Entity& entity, sf::RenderWindow &window){
    if(!entity.shape ||!entity.transform) return;
    window.draw(entity.shape->shape);
  }
};
