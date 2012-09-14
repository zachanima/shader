#version 420
 
precision highp float;

struct Light {
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};
 
in vec2 vertex_texture;
in vec3 vertex_normal;

out vec4 color;

uniform float time;
uniform Light light;
uniform sampler2D sampler;

void main(void) {
  vec4 ambient_color = vec4(light.color, 1.f) * light.ambient;
  float diffuse_factor = dot(normalize(vertex_normal), -light.direction);
  vec4 diffuse_color;

  if (diffuse_factor > 0.f) {
    diffuse_color = vec4(light.color, 1.f) * light.diffuse * diffuse_factor;
  } else {
    diffuse_color = vec4(0.f, 0.f, 0.f, 0.f);
  }

  color = texture2D(sampler, vertex_texture) * (ambient_color + diffuse_color);
}
