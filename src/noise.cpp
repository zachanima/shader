#include "noise.hpp"

module::RidgedMulti Noise::ridged;
module::Billow Noise::billow;
module::ScaleBias Noise::scalebias;
module::Perlin Noise::perlin;
module::Select Noise::select;
module::Turbulence Noise::turbulence;



void Noise::initialize() {
  /*
  ridged.SetOctaveCount(2);
  ridged.SetFrequency(4.f);
  */
  billow.SetFrequency(2.f);
  billow.SetOctaveCount(16);
  /*
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
  return billow.GetValue(r.x, r.y, r.z);
}



void Noise::heightmap(float x1, float y1, float x2, float y2) {
  utils::NoiseMap noisemap;
  utils::NoiseMapBuilderPlane noisemap_builder;
  noisemap_builder.SetSourceModule(ridged);
  noisemap_builder.SetDestNoiseMap(noisemap);
  noisemap_builder.SetDestSize(64, 64);
  noisemap_builder.SetBounds(x1, x2, y1, y2);
  noisemap_builder.Build();

  utils::RendererImage heightmap_renderer;
  utils::Image heightmap;
  heightmap.SetSize(64, 64);
  heightmap_renderer.SetSourceNoiseMap(noisemap);
  heightmap_renderer.SetDestImage(heightmap);
  heightmap_renderer.Render();

  utils::WriterBMP heightmap_writer;
  heightmap_writer.SetSourceImage(heightmap);
  heightmap_writer.SetDestFilename("heightmap.bmp");
  heightmap_writer.WriteDestFile();

  utils::RendererNormalMap normalmap_renderer;
  utils::Image normalmap;
  normalmap.SetSize(64, 64);
  normalmap_renderer.SetSourceNoiseMap(noisemap);
  normalmap_renderer.SetDestImage(normalmap);
  normalmap_renderer.Render();

  utils::WriterBMP normalmap_writer;
  normalmap_writer.SetSourceImage(normalmap);
  normalmap_writer.SetDestFilename("normalmap.bmp");
  normalmap_writer.WriteDestFile();
}
