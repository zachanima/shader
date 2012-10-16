#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;

void main(void) {
  float intensity = texture2D(sampler, vertexTexture).r * texture2D(sampler, vertexTexture).r;
  color = vec4(intensity, intensity, intensity, 1.f);
}
