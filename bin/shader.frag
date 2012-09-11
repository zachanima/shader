#version 420
 
precision highp float;
 
smooth in vec4 vertex_color;
out vec4 color;
uniform float time;

void main(void) {
  vec4 other_color = vec4(vertex_color.g, vertex_color.b, vertex_color.r, vertex_color.a);
  color = mix(vertex_color, other_color, (sin(time) + 1.0) * 0.5);
}
