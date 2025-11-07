#include <Character.h>
#include <GameManager.h>
#include <SFML/Graphics.hpp>
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;
int main() {
  Character Player(WINDOW_WIDTH /2.f , WINDOW_HEIGHT / 2.f, 100.f);
  GameManager gameManager(WINDOW_WIDTH, WINDOW_HEIGHT);
  gameManager.addCharacter(Player);
  gameManager.run();

  return 0;
}
