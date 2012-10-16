#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;

vec3 jet(float value) {
  float f = 4 * value;
  float r = min(f - 1.5f, -f + 4.5f);
  float g = min(f - 0.5f, -f + 3.5f);
  float b = min(f + 0.5f, -f + 2.5f);

  return clamp(vec3(r, g, b), 0.f, 0.75f);
}

void main(void) {
  color = vec4(jet(2.f * texture2D(sampler, vertexTexture).r - 0.5f), 1.f);
}
