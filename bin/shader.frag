#version 420
 
precision highp float;
 
smooth in vec4 vertex_color;
out vec4 color;
uniform float time;

void main(void) {
  color = vertex_color;
}
