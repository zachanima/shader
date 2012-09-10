#version 420
 
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
smooth out vec4 vertex_color;
uniform vec2 offset;
 
void main(void) {
  vec4 total_offset = vec4(sin(offset.x), offset.y, 0.0f, 0.0f);
  gl_Position = position + total_offset;
  vertex_color = color;
}
