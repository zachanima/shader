#version 420
 
layout (location = 0) in vec3 position;
uniform float time;
uniform mat4 perspective;
uniform vec3 camera;

vec3 spherize(vec3 position) {
  vec4 sq = vec4(position, 1.f) * vec4(position, 1.f);
  position.x *= sqrt(1.f - sq.y / 2.f - sq.z /2.f + sq.y * sq.z / 3.f);
  position.y *= sqrt(1.f - sq.x / 2.f - sq.z /2.f + sq.x * sq.z / 3.f);
  position.z *= sqrt(1.f - sq.x / 2.f - sq.y /2.f + sq.x * sq.y / 3.f);
  return position;
}
 
void main(void) {
  vec4 spherized_position = vec4(spherize(position), 1.f);
  vec4 spherized_camera = vec4(spherize(camera), 0.f);
  vec4 final_position = spherized_position + spherized_camera;

  gl_Position = perspective * final_position;
}
