#include "noise.hpp"

module::RidgedMulti Noise::ridged;
module::Billow Noise::billow;
module::ScaleBias Noise::scalebias;
module::Perlin Noise::perlin;
module::Select Noise::select;
module::Turbulence Noise::turbulence;



void Noise::initialize() {
  ridged.SetOctaveCount(20);
  ridged.SetFrequency(4.f);
  /*
  billow.SetFrequency(2.f);
  billow.SetOctaveCount(16);
  scalebias.SetSourceModule(0, billow);
  scalebias.SetScale(0.125f);
  scalebias.SetBias(-0.75f);
  perlin.SetOctaveCount(16);
  perlin.SetFrequency(0.5f);
  perlin.SetPersistence(0.25f);
  select.SetSourceModule(0, scalebias);
  select.SetSourceModule(1, ridged);
  select.SetControlModule(perlin);
  select.SetBounds(0.0f, 1000.0f);
  select.SetEdgeFalloff(0.125f);
  turbulence.SetSourceModule(0, select);
  turbulence.SetFrequency(4.0f);
  turbulence.SetPower(0.125f);
  */
}



const float Noise::noise(glm::vec3 r) {
  return ridged.GetValue(r.x, r.y, r.z);
}
