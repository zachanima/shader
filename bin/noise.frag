#version 420

out vec4 color;

void main(void) {
  color = vec4((1.f + sin(gl_FragCoord.x / 4.f)) / 2.f, 1.f, 1.f, 1.f);
}
