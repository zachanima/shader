#ifndef CAMERA_HPP
#define CAMERA_HPP

struct Camera {
  // Attributes.
  vec3 position;
  vec3 target;
  quat orientation;

  // Uniforms.
  GLuint positionUniform;
};

#endif // CAMERA_HPP
