#include <Entity.h>

#include <GameManager.h>
#include <SFML/Graphics.hpp>
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;
int main() {
  GameManager game(WINDOW_WIDTH,WINDOW_HEIGHT);

  Entity player;
  player.transform = Transform(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
  player.velocity = Velocity(); 
  player.movement = Movement();
  player.jump = JumpComponent();
  player.collider = Collider(20.f,50.f);
  player.shape = Shape(sf::Color::Blue,20.f,50.f);

  Entity platform;
  platform.transform = Transform(900.f,900.f);
  platform.collider = Collider(100.f,100.f,true);
  platform.shape = Shape(sf::Color::Red, 100.f, 100.f);

  game.addEntity(player);
  game.addEntity(platform);
  
  game.run();
  return 0;
}
