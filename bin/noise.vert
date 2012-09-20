#version 420

layout (location = 0) in vec3 position;

out vec3 vertexPosition;

void main(void) {
  gl_Position = vec4(position, 1.f);
  vertexPosition = position;
}
