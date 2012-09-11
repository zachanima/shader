#ifndef GAME_HPP
#define GAME_HPP

#define WIDTH (1280)
#define HEIGHT (720)

#include "lwcppgl/display.hpp"

#include "source.hpp"

class Game {
  public:
    static GLvoid initialize();
    static GLvoid render();

  private:
    static GLuint VAO;
    static GLuint program;
    static GLuint time_uniform;
    static const unsigned int INDICES;
};

#endif // GAME_HPP
