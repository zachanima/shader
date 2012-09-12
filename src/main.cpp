#include "main.hpp"

int main(int argc, char **argv) {
  Display::create(WIDTH, HEIGHT);
  Game::initialize();

  while (!Keyboard::isKeyDown(KEY_ESCAPE)) {
    Game::update();
    Game::render();
    Display::update();
  }

  return 0;
}
