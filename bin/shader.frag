#version 420
 
precision highp float;

struct Light {
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};
 
in vec3 vertex_normal;

out vec4 color;

uniform float time;
uniform Light light;

void main(void) {
  vec4 ambient_color = vec4(light.color, 1.f) * light.ambient;
  float diffuse_factor = dot(normalize(vertex_normal), -light.direction);
  vec4 diffuse_color;

  if (diffuse_factor > 0.f) {
    diffuse_color = vec4(light.color, 1.f) * light.diffuse * diffuse_factor;
  } else {
    diffuse_color = vec4(0.f, 0.f, 0.f, 0.f);
  }

  color = vec4(1.f, 1.f, 1.f, 1.f) * (ambient_color + diffuse_color);
}
