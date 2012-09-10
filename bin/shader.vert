#version 420
 
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
smooth out vec4 vertex_color;
uniform float time;
 
void main(void) {
  vec4 total_offset = vec4(sin(time) * 0.5, cos(time * 0.7) * 0.5, 0.0f, 0.0f);
  gl_Position = position + total_offset;
  vertex_color = color;
}
