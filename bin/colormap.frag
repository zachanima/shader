#version 140

in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform sampler2D sampler;

float scale(float value, float srcmin, float srcmax) {
  return ((value - srcmin) / (srcmax - srcmin));
}

void main(void) {
  float height = 2.f * (texture2D(sampler, vertexTexture).r - 0.5f);

  vec4 deeps =   vec4(0.f,   0.f,   0.2f, 1.f);
  vec4 shallow = vec4(0.f,   0.f,   0.5f,  1.f);
  vec4 shore =   vec4(0.f,   0.2f,  0.5f,  1.f);
  vec4 sand =    vec4(0.5f,  0.5f,  0.1f, 1.f);
  vec4 grass =   vec4(0.05f, 0.3f, 0.f,  1.f);
  vec4 dirt =    vec4(0.4f,  0.4f, 0.f,  1.f);
  vec4 rock =    vec4(0.2f,  0.2f, 0.2f, 1.f);
  vec4 snow =    vec4(1.f,   1.f,  1.f,  1.f);

  if (height < -0.25) {          color = mix(deeps,   shallow, scale(height, -1.f, -0.25f));
  } else if (height < 0.f) {     color = mix(shallow, shore,   scale(height, -0.25f, 0.f));
  } else if (height < 0.0625f) { color = mix(shore,   sand,    scale(height, 0.f, 0.0625f));
  } else if (height < 0.125f) {  color = mix(sand,    grass,   scale(height, 0.0625f, 0.125f));
  } else if (height < 0.375f) {  color = mix(grass,   dirt,    scale(height, 0.125f, 0.375f));
  } else if (height < 0.5f) {    color = mix(dirt,    rock,    scale(height, 0.375f, 0.5f));
  } else {                       color = mix(rock,    snow,    scale(height, 0.5f, 1.f));
  }
}
