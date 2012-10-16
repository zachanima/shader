#version 140


// Ashima Arts Simplex 3D implementation.

vec3 mod289(vec3 x) {
  return x - floor(x * (1.f / 289.f)) * 289.f;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.f / 289.f)) * 289.f;
}

vec4 permute(vec4 x) {
  return mod289((x * 34.f + 1.f) * x);
}

vec4 taylorInvSqrt(vec4 r) {
  return 1.79284291400159f - 0.85373472095314f * r;
}

float snoise(vec3 v) {
  const vec2 C = vec2(1.f / 6.f, 1.f / 3.f);
  const vec4 D = vec4(0.f, 0.5f, 1.f, 2.f);

  // First corner.
  vec3 i = floor(v + dot(v, C.yyy));
  vec3 x0 = v - i + dot(i, C.xxx);

  // Other corners.
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1. - g;
  vec3 i1 = min(g.xyz, l.zxy);
  vec3 i2 = max(g.xyz, l.zxy);

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;

  // Permutations.
  i = mod289(i);
  vec4 p = permute(permute(permute(
    i.z + vec4(0.f, i1.z, i2.z, 1.f)) +
    i.y + vec4(0.f, i1.y, i2.y, 1.f)) +
    i.x + vec4(0.f, i1.x, i2.x, 1.f)
  );

  // Gradients.
  float n_ = 0.142857142857f;
  vec3 ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0f * floor(p * ns.z * ns.z);

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0f * x_ );

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0f - abs(x) - abs(y);

  vec4 b0 = vec4(x.xy, y.xy);
  vec4 b1 = vec4(x.zw, y.zw);

  vec4 s0 = floor(b0) * 2.0f + 1.0f;
  vec4 s1 = floor(b1) * 2.0f + 1.0f;
  vec4 sh = -step(h, vec4(0.0f));

  vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  //Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  // Mix final noise value
  vec4 m = max(0.6f - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;

  return 42.0f * dot(m * m, vec4(
    dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3)
  ));
}

float ridge(float h, float offset) {
  h = abs(h);
  h = offset - h;
  h = h * h;
  return h;
}

float simplex(vec3 v, int octaves, float persistence, float scale) {
  float total = 0.f;
  float frequency = scale;
  float amplitude = 1.f;

  float maxAmplitude = 0.f;

  for (int i = 0; i < octaves; i++) {
    total += ridge(snoise(v * frequency), 1.f) * amplitude;
    frequency *= 2.f;
    maxAmplitude += amplitude;
    amplitude *= persistence;
  }

  return total / maxAmplitude;
}

in vec3 vertexPosition;

out vec4 color;

void main(void) {
  float noise = simplex(vertexPosition, 24, 0.5f, 0.5f);
  color = vec4(noise, noise, noise, 1.f);
}
