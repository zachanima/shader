#version 420
 
precision highp float;
 
smooth in vec4 vertex_color;
out vec4 color;
 
void main(void) {
  color = vertex_color;
}
