#version 420
 
precision highp float;
 
out vec4 color;
 
void main(void) {
  float lerpValue = gl_FragCoord.y / 360.0f;
  color = mix(vec4(1.0f, 1.0f, 1.0f, 0.5f),
    vec4(0.2f, 0.2f, 0.2f, 0.5f), lerpValue);
}
