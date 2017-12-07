#include "LowPolyPt.h"

float LowPolyPt::radius = 50.0f;
std::function<glm::vec3(glm::vec3, float)> LowPolyPt::colorInterpolateFunc 
    = [](glm::vec3 coord, float radius) { return glm::vec3(0.0f, 0.0f, 0.0f); };

void LowPolyPt::setColorInterpolateFunc(std::function<glm::vec3(glm::vec3, float)> func) {
  LowPolyPt::colorInterpolateFunc = func;
}

void LowPolyPt::setRadiusScale(float r) {
  radius = r;
}

LowPolyPt::LowPolyPt(glm::vec3 vert) {
  for (int i = 0; i < 6; i++) {
    pts[i].vertex = vert;
    pts[i].color = colorInterpolateFunc(vert, radius);
    pts[i].used = 0;
  }
}

int LowPolyPt::setNorm(glm::vec3 norm) {
  int i = 0;
  for (; i < 6; i++) {
    if (!pts[i].used)
      break;
  }

  // all norm set
  if (i == 6)
    return -1;

  pts[i].norm = norm;
  pts[i].used = 1;

  return i;
}

glm::vec3 LowPolyPt::getCoord() {
  return pts[0].vertex;
}

