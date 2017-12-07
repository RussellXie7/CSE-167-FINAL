#include "TerrainColorGen.h"

std::default_random_engine TerrainColorGen::gen(230815);
std::normal_distribution<float> TerrainColorGen::dist(0.0f, 0.5f);

glm::vec3 TerrainColorGen::colors[8] = {
  glm::vec3(0.7f, 0.7f, 0.7f),          // rock grey
  glm::vec3(0.184f, 0.494f, 0.196f),    // dark green
  glm::vec3(0.454f, 0.765f, 0.290f),    // light green
  glm::vec3(0.737f, 0.667f, 0.643f),    // light brown
  glm::vec3(0.365f, 0.251f, 0.216f),    // dark brown
  glm::vec3(0.129f, 0.129f, 0.129f)     // charcoal
};

float TerrainColorGen::scales[4] = {0.3f, 0.0f, -0.9f};   

glm::vec3 TerrainColorGen::getColor(glm::vec3 coord, float radius) {

  // possible interpolating color
  glm::vec3 i0, i1;
  float scale = (coord.y + dist(gen))/ radius;
  
  if (scale > scales[0]) {
    return colors[0];
  
  } else if (scale > scales[1]) {
    i0 = colors[1];
    i1 = colors[2];
    scale = (scales[0] - scale) / (scales[0] - scales[1]);
  
  } else if (scale > scales[2]) {
    i0 = colors[3];
    i1 = colors[4];
    scale = (scales[1] - scale) / (scales[1] - scales[2]);
    
  } else {
    return colors[7];
  }

  return i0 * (1.0f - scale) + i1 * scale;
}


