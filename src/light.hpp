#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

using namespace glm;

struct Light {
  vec3 color;
  GLfloat ambient;
};

#endif // LIGHT_HPP
