#version 140

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;

out vec3 vertexPosition;
out vec2 vertexTexture;

void main(void) {
  gl_Position = vec4(position, 1.f);
  vertexPosition = position;
  vertexTexture = texture;
}
