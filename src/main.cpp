#include <Entity.h>

#include <GameManager.h>
#include <SFML/Graphics.hpp>
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;
int main() {
  GameManager game(WINDOW_WIDTH, WINDOW_HEIGHT);

  Entity player;
  player.transform = Transform(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
  player.velocity = Velocity();
  player.movement = Movement();
  player.jump = JumpComponent();
  player.collider = Collider(20.f, 50.f);
  player.shape = Shape(sf::Color::Blue, 20.f, 50.f);
  player.tag = Tag(EntityLayer::Player);
  player.grounded = Grounded();
  player.dash = Dash();

  Entity platform;
  platform.transform = Transform(300.f, 700.f);
  platform.collider = Collider(400.f, 100.f, true);
  platform.shape = Shape(sf::Color::Red, 400.f, 100.f);
  platform.tag = Tag(EntityLayer::Ground);

  Entity platform2;
  platform2.transform = Transform(1000.f, 700.f);
  platform2.collider = Collider(700.f, 100.f, true);
  platform2.shape = Shape(sf::Color::Red, 700.f, 100.f);
  platform2.tag = Tag(EntityLayer::Ground);
  game.addEntity(player);
  game.addEntity(platform);
  game.addEntity(platform2);

  game.run();
  return 0;
}
