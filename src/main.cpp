#include <Entity.h>

#include <GameManager.h>
#include <SFML/Graphics.hpp>
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;
int main() {
  GameManager game(WINDOW_WIDTH, WINDOW_HEIGHT);

  std::cout << "Current working directory: " << std::filesystem::current_path()
            << std::endl;
  Entity player;
  player.transform = Transform(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
  player.velocity = Velocity();
  player.movement = Movement();
  player.jump = JumpComponent();
  player.tag = Tag(EntityLayer::Player);
  player.grounded = Grounded();
  player.dash = Dash();

  sf::Texture *playerTexture = game.getTexture("player");
  if (playerTexture) {
    std::cout << "✓ Player texture found" << std::endl;
    EntityAnimConfig *config = game.getAnimConfig("player");
    player.sprite = Sprite(*playerTexture, config->scale);
    player.collider = Collider(100.f, 100.f);
    if (config) {
      std::cout << "✓ Animation config found" << std::endl;
      Animation anim;
      anim.animations = config->animations;
      anim.currentAnimation = "idle";
      anim.currentFrame = 0;
      anim.frameTimer = 0.f;
      player.animation = anim;
      auto &idleAnim = config->animations["idle"];
      player.sprite->sprite.setTextureRect(
          sf::IntRect({idleAnim.startX, idleAnim.startY},
                      {(int)idleAnim.frameWidth, (int)idleAnim.frameHeight}));
      player.sprite->sprite.setOrigin(
          {idleAnim.frameWidth / 2.f, idleAnim.frameHeight / 2.f});
      std::cout << "✓ Animation set to: " << anim.currentAnimation << std::endl;
    }
  }
  Entity platform;
  platform.transform = Transform(700.f, 700.f);
  platform.collider = Collider(1400.f, 100.f, true);
  platform.shape = Shape(sf::Color::Red, 1400.f, 100.f);
  platform.tag = Tag(EntityLayer::Ground);

  game.addEntity(player);
  game.addEntity(platform);

  game.run();
  return 0;
}
