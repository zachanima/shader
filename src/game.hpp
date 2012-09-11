#ifndef GAME_HPP
#define GAME_HPP

#include "lwcppgl/display.hpp"

#include "source.hpp"
#include "quadtree.hpp"

#define WIDTH (1280)
#define HEIGHT (720)

class Game {
  public:
    static GLvoid initialize();
    static GLvoid render();

  private:
    static GLuint vao;
    static GLuint program;
    static GLuint time_uniform;
    static Quadtree *quadtree;
};

#endif // GAME_HPP
