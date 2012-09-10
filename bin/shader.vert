#version 420
 
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
smooth out vec4 vertex_color;
uniform float time;
uniform mat4 perspective;
 
void main(void) {
  vec4 offset = vec4(sin(time) * 0.75, cos(time * 0.7) * 0.75, 0.0f, 0.0f);
  vec4 camera = position + vec4(offset);

  gl_Position = perspective * camera;
  vertex_color = color;
}
