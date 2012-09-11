#version 420
 
precision highp float;
 
out vec4 color;
uniform float time;

void main(void) {
  color = mix(vec4(0.f, 0.f, 0.f, 1.f), vec4(1.f, 1.f, 1.f, 1.f), (sin(time) + 1.f) * 0.5f);
}
