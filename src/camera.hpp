#ifndef CAMERA_HPP
#define CAMERA_HPP

struct Camera {
  // Attributes.
  vec3 position;
  quat orientation;

  // Uniforms.
  GLuint positionUniform;
};

#endif // CAMERA_HPP
