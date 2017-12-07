#include "SkyboxColorGen.h"

std::default_random_engine SkyboxColorGen::gen(230815);
std::normal_distribution<float> SkyboxColorGen::dist(0.0f, 0.5f);

glm::vec3 SkyboxColorGen::colors[4] = {
  glm::vec3(0.149f, 0.196f, 0.220f),          // space grey
  glm::vec3(0.118f, 0.533f, 0.898f),          // sky blue
  glm::vec3(0.98f, 0.98f, 0.98f),             // color white
  glm::vec3(0.999f, 0.702f, 0.0f)             // sun
};

float SkyboxColorGen::scale = -0.2f;   

glm::vec3 SkyboxColorGen::sun = glm::vec3(0.25f, 0.859f, -0.429f);

glm::vec3 SkyboxColorGen::getColor(glm::vec3 coord, float radius) {

  // possible interpolating color
  glm::vec3 i0, i1;
  float alpha = (coord.y + dist(gen))/ radius;
  
  if (scale > scale) {
    i0 = colors[0];
    i1 = colors[1];
    alpha = (1.0f - alpha) / (1.0f - scale);
  
  } else {
    i0 = colors[1];
    i1 = colors[2];
    scale = (scale - alpha) / (scale + 1.0f);
  } 

  // if near sun
  if (glm::distance(sun, glm::normalize(coord)) < 0.33f)
    return colors[3];

  return i0 * (1.0f - alpha) + i1 * alpha;
}



