#ifndef GAME_HPP
#define GAME_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "lwcppgl/display.hpp"

#include "quadtree.hpp"
#include "light.hpp"
#include "camera.hpp"

#define WIDTH (1280)
#define HEIGHT (720)

using namespace glm;

class Game {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static Camera camera;
    static Light light;
    static GLuint program;
    static GLuint viewUniform;
    static Quadtree *quadtree;
    static Quadtree *water;
    static GLvoid offsetOrientation(vec3, GLfloat);
    static const mat4 lookAt(vec3, vec3, vec3);
};

#endif // GAME_HPP
