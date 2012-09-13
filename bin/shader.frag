#version 420
 
precision highp float;

struct Light {
  vec3 color;
  float ambient;
};
 
smooth in vec4 vertex_color;
out vec4 color;
uniform float time;
uniform Light light;


void main(void) {
  color = vertex_color * vec4(light.color, 1.f) * light.ambient;
}
