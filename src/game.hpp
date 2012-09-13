#ifndef GAME_HPP
#define GAME_HPP

#include "lwcppgl/display.hpp"

#include "noise.hpp"
#include "source.hpp"
#include "quadtree.hpp"
#include "light.hpp"

#define WIDTH (1280)
#define HEIGHT (720)

class Game {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLfloat camera[3];
    static Light light;
    static GLuint program;
    static GLuint time_uniform;
    static GLuint camera_uniform;
    static GLuint light_color_uniform;
    static GLuint light_ambient_uniform;
    static Quadtree *quadtree;
    static Quadtree *water;
};

#endif // GAME_HPP
