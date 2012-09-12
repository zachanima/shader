#ifndef NOISE_HPP
#define NOISE_HPP

#include <libnoise/noise.h>

class Noise {
  public:
    static void initialize();
    static const float noise(const float, const float, const float);

  private:
    static noise::module::RidgedMulti ridged;
    static noise::module::Billow billow;
    static noise::module::ScaleBias scalebias;
    static noise::module::Perlin perlin;
    static noise::module::Select select;
    static noise::module::Turbulence turbulence;
};

#endif // NOISE_HPP
