#version 140

layout (location = 0) in vec3 position;

out vec3 vertexPosition;

uniform vec3 meshPosition;
uniform float meshLength;

void main(void) {
  gl_Position = vec4(position, 1.f);
  vec3 normalizedPosition = (position + vec3(1.f, 1.f, 0.f)) / 2.f;
  vec3 computedPosition = meshPosition;
  vertexPosition = computedPosition + vec3(meshLength, meshLength, 1.f) * normalizedPosition;
}
