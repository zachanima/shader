#version 420
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vertex_normal;
smooth out vec4 vertex_color;

uniform float time;
uniform mat4 perspective;
uniform vec3 camera;

void main(void) {
  vec4 final_position = vec4(position, 1.f) - vec4(camera, 0.f);
  float dist = distance(vec4(position, 1.f), vec4(0.f, 0.f, 0.f, 1.f));

  gl_Position = perspective * final_position;
  vertex_normal = (perspective * vec4(normal, 0.f)).xyz;
  vertex_color = vec4(1.f, 1.f, 1.f, 1.f);
}
