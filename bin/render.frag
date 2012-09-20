#version 420
 
precision highp float;

struct Light {
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};
 
in vec3 vertexPosition;
in vec3 vertexNormal;

out vec4 color;

uniform Light light;

void main(void) {
  vec4 ambientColor = vec4(light.color, 1.f) * light.ambient;
  float diffuseFactor = dot(normalize(vertexNormal), -light.direction);
  vec4 diffuseColor;

  if (diffuseFactor > 0.f) {
    diffuseColor = vec4(light.color, 1.f) * light.diffuse * diffuseFactor;
  } else {
    diffuseColor = vec4(0.f, 0.f, 0.f, 0.f);
  }

  color = vec4(1.f, 1.f, 1.f, 1.f) * (ambientColor + diffuseColor);
}
