#version 420
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

out vec3 vertexPosition;
out vec3 vertexNormal;

uniform mat4 perspective;
uniform vec3 camera;

vec3 spherize(vec3 r) {
  const vec3 r2 = r * r;
  r.x *= sqrt(1.f - r2.y / 2.f - r2.z / 2.f + r2.y * r2.z / 3.f);
  r.y *= sqrt(1.f - r2.x / 2.f - r2.z / 2.f + r2.x * r2.z / 3.f);
  r.z *= sqrt(1.f - r2.x / 2.f - r2.y / 2.f + r2.x * r2.y / 3.f);
  return r;
}

void main(void) {
  vec4 finalPosition = vec4(spherize(position), 1.f) - vec4(camera, 0.f);

  gl_Position = perspective * finalPosition;
  vertexNormal = (perspective * vec4(normal, 0.f)).xyz;
  vertexPosition = position;
}
