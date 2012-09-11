#version 420
 
layout (location = 0) in vec3 position;
uniform float time;
uniform mat4 perspective;

vec4 spherize(vec3 position) {
  vec4 sq = vec4(position, 1.f) * vec4(position, 1.f);
  position.x *= sqrt(1.f - sq.y / 2.f - sq.z /2.f + sq.y * sq.z / 3.f);
  position.y *= sqrt(1.f - sq.x / 2.f - sq.z /2.f + sq.x * sq.z / 3.f);
  position.z *= sqrt(1.f - sq.x / 2.f - sq.y /2.f + sq.x * sq.y / 3.f);
  return vec4(position, 1.f);
}
 
void main(void) {
  vec4 offset = vec4(
    sin(time * 0.9f) * 0.75f,
    cos(time * 0.7f) * 0.75f,
    -4.f + sin(time * 0.3f),
    0.f
  );
  vec4 position2 = mix(vec4(position, 1.f), spherize(position), sin(time));
  vec4 camera = position2 + vec4(offset);

  gl_Position = perspective * camera;
}
