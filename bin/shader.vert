#version 420
 
layout (location = 0) in vec3 position;
uniform float time;
uniform mat4 perspective;
uniform vec3 camera;
smooth out vec4 vertex_color;

void main(void) {
  vec4 final_position = vec4(position, 1.f) - vec4(camera, 0.f);
  float dist = distance(vec4(position, 1.f), vec4(0.f, 0.f, 0.f, 1.f));

  gl_Position = perspective * final_position;
  vertex_color = mix(
    vec4(0.2f, 0.3f, 0.8f, 0.75f),
    vec4(1.0f, 1.0f, 1.0f, 1.0f),
    (dist - 1.f) * 32.f
  );
}
