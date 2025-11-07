#pragma once
#include <SFML/Graphics.hpp>
#include <Character.h>
#include <Components.h>


class RenderSystem{
public:
  void renderCharacter(Character& character, sf::RenderWindow& window){
    character.draw(window);
  }
};
