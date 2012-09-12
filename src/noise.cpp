#include "noise.hpp"

noise::module::RidgedMulti Noise::ridged;
noise::module::Billow Noise::billow;
noise::module::ScaleBias Noise::scalebias;
noise::module::Perlin Noise::perlin;
noise::module::Select Noise::select;
noise::module::Turbulence Noise::turbulence;



void Noise::initialize() {
  ridged.SetOctaveCount(20);
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
}



const float Noise::noise(const float x, const float y, const float z) {
  return turbulence.GetValue(x, y, z);
}
