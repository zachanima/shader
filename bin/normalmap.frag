#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;
uniform int level;
uniform float textureSize;

const float normalStrength = 12.f;

void main(void) {
  const float texel = 1.f / textureSize;
  float intensity = 65536 >> level;
  float tl;
  float l;
  float bl;
  float t;
  float b;
  float tr;
  float r;
  float br;

  // Texture offsets for edges of normal map.
  vec2 offset = vec2(0.f, 0.f);
  if (vertexTexture.s < texel)       { offset.x =  1.f; }
  if (vertexTexture.s > 1.f - texel) { offset.x = -1.f; }
  if (vertexTexture.t < texel)       { offset.y =  1.f; }
  if (vertexTexture.t > 1.f - texel) { offset.y = -1.f; }

  tl = abs(texture2D(sampler, vertexTexture.st + texel * vec2(-1.f + offset.x, -1.f + offset.y)).x) * intensity;
  l  = abs(texture2D(sampler, vertexTexture.st + texel * vec2(-1.f + offset.x,  0.f + offset.y)).x) * intensity;
  bl = abs(texture2D(sampler, vertexTexture.st + texel * vec2(-1.f + offset.x,  1.f + offset.y)).x) * intensity;
  t  = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 0.f + offset.x, -1.f + offset.y)).x) * intensity;
  b  = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 0.f + offset.x,  1.f + offset.y)).x) * intensity;
  tr = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 1.f + offset.x, -1.f + offset.y)).x) * intensity;
  r  = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 1.f + offset.x,  0.f + offset.y)).x) * intensity;
  br = abs(texture2D(sampler, vertexTexture.st + texel * vec2( 1.f + offset.x,  1.f + offset.y)).x) * intensity;

  float dX = tr + 2.f * r + br - tl - 2.f * l - bl;
  float dY = bl + 2.f * b + br - tl - 2.f * t - tr;

  vec4 normal = vec4(normalize(vec3(dX, dY, -1.f / normalStrength)), 1.f);

  normal *= 0.5f;
  normal += 0.5f;

  color = normal;
}
