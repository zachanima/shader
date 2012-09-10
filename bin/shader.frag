#version 420
 
precision highp float;
 
smooth in vec4 vertex_color;
out vec4 color;
uniform float time;

void main(void) {
  vec4 other_color = vec4(vertex_color.y, vertex_color.z, vertex_color.x, vertex_color.w);
  color = mix(vertex_color, other_color, (sin(time) + 1.0) * 0.5);
}
