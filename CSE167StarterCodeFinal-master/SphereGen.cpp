#include "SphereGen.h"

std::default_random_engine SphereGen::gen(3623);
std::normal_distribution<float> SphereGen::dist(0.0f, 0.5f);

glm::vec3 SphereGen::getHeightUpper(float x, float z, float radius) {

  float y = fabs(x) > fabs(z)? fabs(x): fabs(z);
  y = 1.2f * pow((radius - y), 1.2f) + dist(gen);

  // glm::vec3 coord = glm::vec3(sphereX, sphereY, sphereZ);
  
  // float scale = 0.4f * radius * (0.8f - x * x / radius / radius ) * (0.8f - z * z / radius / radius) *dist(gen);
  float scale = 0.4f * radius * (0.8f - x * x / radius / radius ) * (0.8f - z * z / radius / radius) *dist(gen);
  if (scale > 0.5f * y || scale < -0.5f * y) scale = 0.0f;

  y += scale;

  glm::vec3 coord = glm::vec3(x, y, z);

  return coord;
}

glm::vec3 SphereGen::getHeightLower(float x, float z, float radius) {
  return -getHeightUpper(x, z, radius);
}
