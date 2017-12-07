#include "TerrainGen.h"

std::default_random_engine TerrainGen::gen(0);
std::default_random_engine TerrainGen::genDist(8326023);
std::normal_distribution<float> TerrainGen::dist(0.0f, 0.5f);

/**
 * public function, to generate height
 */
glm::vec3 TerrainGen::getHeight(float x, float z, float radius) {

  float y = 0.0f;
  float d = float(pow(2, OCTAVES - 1));
  float amplitude = radius;

  // mix wave of decreasing amplitude and increase frequency
  for (int i = 0; i < 5; i++) {
    float freq = float(pow(2, i)) / d;
    float amp = float(pow(0.5f, i)) * amplitude;

    y += getInterpolatedNoise((x + radius) * freq, (z + radius) * freq, radius) * amp;
  }

  y += -0.8f * radius * (0.8 - fabs(x) / radius) * (0.8 - fabs(z) / radius);
  y += 0.2f * radius;

  // randomly move x, y a little bit
  float dx = dist(genDist);
  dx = fabs(dx) > 0.5f? 0.0f: dx; 
  float dz = dist(genDist);
  dz = fabs(dx) > 0.5f? 0.0f: dz; 

  if (fabs(x - radius) < 0.5f || fabs(x + radius) < 0.5f || fabs(z - radius) < 0.5f || fabs(z + radius) < 0.5f)
    return glm::vec3(x + dx, dx * dz / 2.0f, z + dz);
  else
    return glm::vec3(x + dx, y, z + dz);
}
 
/**
 * use sine interpolate to smooth shrinked terrain
 */
float TerrainGen::getInterpolatedNoise(float x, float z, float radius) {
  int intX = (int) x;
  int intZ = (int) z;
  float fracX = x - intX;
  float fracZ = z - intZ;

  float p00 = getSmoothNoise(intX, intZ, radius);
  float p01 = getSmoothNoise(intX + 1, intZ, radius);
  float p10 = getSmoothNoise(intX, intZ + 1, radius);
  float p11 = getSmoothNoise(intX + 1, intZ + 1, radius);

  // bilinear interpolation
  float upper = interpolate(p00, p01, fracX);
  float lower = interpolate(p10, p11, fracX);
  
  return interpolate(upper, lower, fracZ);
}

/**
 * sine interpolate
 */
float TerrainGen::interpolate(float v0, float v1, float alpha) {
  float theta = alpha * 3.1416f;
  float f = (1.0f - float(cos(theta))) * 0.5f;
  return v0 * (1.0f -f) + v1 * f;
}

/**
 * smooth noise at point by taking noise of surrounding point
 */ 
float TerrainGen::getSmoothNoise(int x, int z, float radius) {
  float corners = (getNoise(x - 1, z - 1, radius) + getNoise(x - 1, z + 1, radius) 
      + getNoise(x + 1, z - 1, radius) + getNoise(x + 1, z + 1, radius)) / 16.0f;
  float sides = (getNoise(x - 1, z, radius) + getNoise(x + 1, z, radius) 
      + getNoise(x, z - 1, radius) + getNoise(x, z + 1, radius)) / 8.0f;
  float center = getNoise(x, z, radius) / 4.0f;

  return corners + sides + center;
}

/**
 * random noise
 */
float TerrainGen::getNoise(int x, int z, float radius) {
  gen.seed(int(x * 23589 + z * 695723 + 18238));
  
  float height = float(gen()) / float(gen.max());
  // make sure terrain edge is float
  return height * 2.0f - 1.0f;
}
