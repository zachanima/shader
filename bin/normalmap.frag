#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;
uniform int level;

const float texel = 1.f / 256.f;
const float normalStrength = 12.f;

void main(void) {
  float intensity = 65536 >> level;
  float tl = abs(texture2D(sampler, vertexTexture.st + texel * vec2(-1.f, -1.f)).x) * intensity;
  float l  = abs(texture2D(sampler, vertexTexture.st + texel * vec2(-1.f,  0.f)).x) * intensity;
  float bl = abs(texture2D(sampler, vertexTexture.st + texel * vec2(-1.f,  1.f)).x) * intensity;
  float t  = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 0.f, -1.f)).x) * intensity;
  float b  = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 0.f,  1.f)).x) * intensity;
  float tr = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 1.f, -1.f)).x) * intensity;
  float r  = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 1.f,  0.f)).x) * intensity;
  float br = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 1.f,  1.f)).x) * intensity;

  float dX = tr + 2.f * r + br - tl - 2.f * l - bl;
  float dY = bl + 2.f * b + br - tl - 2.f * t - tr;

  vec4 normal = vec4(normalize(vec3(dX, dY, 1.f / normalStrength)), 1.f);

  normal *= 0.5f;
  normal += 0.5f;

  color = normal;
}
