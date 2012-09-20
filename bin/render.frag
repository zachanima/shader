#version 420
 
precision highp float;

struct Light {
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};
 
in vec3 vertexPosition;
in vec2 vertexTexture;
in vec3 vertexNormal;

out vec4 color;

uniform Light light;
uniform sampler2D sampler;

void main(void) {
  vec4 ambientColor = vec4(light.color, 1.f) * light.ambient;
  float diffuseFactor = dot(normalize(vertexNormal), -light.direction);
  vec4 diffuseColor;

  if (diffuseFactor > 0.f) {
    diffuseColor = vec4(light.color, 1.f) * light.diffuse * diffuseFactor;
  } else {
    diffuseColor = vec4(0.f, 0.f, 0.f, 0.f);
  }

  color = texture2D(sampler, vertexTexture) * (ambientColor + diffuseColor);
}
