#pragma once
#include <Character.h>
#include <Components.h>
#include <Platform.h>
#include <SFML/Graphics.hpp>

class RenderSystem {
public:
  void renderCharacter(Character &character, sf::RenderWindow &window) {
    character.getShape().shape.setPosition(character.getTransform().toVector());
    character.draw(window);
  }
  void renderPlatform(Platform &platform, sf::RenderWindow &window) {
    platform.getShape().shape.setPosition(platform.getTransform().toVector());
    platform.draw(window);
  }
};
