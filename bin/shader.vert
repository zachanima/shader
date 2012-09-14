#version 420
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

out vec2 vertex_texture;
out vec3 vertex_normal;

uniform float time;
uniform mat4 perspective;
uniform vec3 camera;

void main(void) {
  vec4 final_position = vec4(position, 1.f) - vec4(camera, 0.f);
  float dist = distance(vec4(position, 1.f), vec4(0.f, 0.f, 0.f, 1.f));

  gl_Position = perspective * final_position;
  vertex_texture = texture;
  vertex_normal = (perspective * vec4(normal, 0.f)).xyz;
}
