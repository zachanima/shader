#version 140
 
struct Light {
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};
 
in vec3 vertexPosition;
in vec2 vertexTexture;
in vec3 vertexNormal;

out vec4 color;

uniform Light light;
uniform sampler2D normalmap;
uniform sampler2D colormap;

void main(void) {
  float distSqr = dot(light.direction, light.direction);
  vec3 lVec = light.direction * inversesqrt(distSqr);
  vec4 base = vec4(1.f, 1.f, 1.f, 1.f);
  vec3 bump = normalize(texture2D(normalmap, vertexTexture).xyz * 2.f - 1.f);
  vec4 vAmbient = vec4(light.ambient * vec3(1.f, 1.f, 1.f), 1.f);
  float diffuse = max(dot(lVec, bump + vertexNormal), 0.f);
  vec4 vDiffuse = vec4(light.diffuse * vec3(1.f, 1.f, 1.f) * diffuse, 1.f);

  color = texture2D(colormap, vertexTexture) * vec4(vAmbient * base + vDiffuse * base);
}
