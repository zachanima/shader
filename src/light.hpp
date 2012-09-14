#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

using namespace glm;

struct Light {
  // Attributes.
  vec3 direction;
  vec3 color;
  GLfloat ambient;
  GLfloat diffuse;

  // Uniforms.
  GLuint directionUniform;
  GLuint colorUniform;
  GLuint ambientUniform;
  GLuint diffuseUniform;
};

#endif // LIGHT_HPP
