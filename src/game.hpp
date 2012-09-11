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
    static GLuint vao;
    static GLuint program;
    static GLuint time_uniform;
    static const GLuint INDICES;
};

#endif // GAME_HPP
