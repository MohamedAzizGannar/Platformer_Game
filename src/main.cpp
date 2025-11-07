#include <Character.h>
#include <GameManager.h>
#include<Platform.h>
#include <SFML/Graphics.hpp>
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;
int main() {
  GameManager gameManager(WINDOW_WIDTH, WINDOW_HEIGHT);
  gameManager.addCharacter(
    std::make_unique<Character>(WINDOW_WIDTH/2.f,0.f, 100.f)
  );
  gameManager.addPlatform( std::make_unique<Platform>(WINDOW_WIDTH/2.f,WINDOW_HEIGHT * 0.8f,WINDOW_WIDTH, WINDOW_HEIGHT * 0.5f));
  gameManager.run();

  return 0;
}
