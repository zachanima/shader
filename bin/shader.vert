#version 420
 
layout (location = 0) in vec3 position;
uniform float time;
uniform mat4 perspective;
uniform vec3 camera;

void main(void) {
  vec4 final_position = vec4(position, 1.f) - vec4(camera, 0.f);

  gl_Position = perspective * final_position;
}
