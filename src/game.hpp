#ifndef GAME_HPP
#define GAME_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>

#include "lwcppgl/display.hpp"

#include "noise.hpp"
#include "source.hpp"
#include "quadtree.hpp"
#include "light.hpp"

#define WIDTH (1280)
#define HEIGHT (720)

using namespace glm;

class Game {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static vec3 camera;
    static Light light;
    static GLuint program;
    static GLuint time_uniform;
    static GLuint camera_uniform;
    static GLuint light_direction_uniform;
    static GLuint light_color_uniform;
    static GLuint light_ambient_uniform;
    static GLuint light_diffuse_uniform;
    static Quadtree *quadtree;
    static Quadtree *water;
};

#endif // GAME_HPP
