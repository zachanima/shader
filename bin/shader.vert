#version 420
 
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
smooth out vec4 vertex_color;
 
void main(void) {
  gl_Position = position;
  vertex_color = color;
}
