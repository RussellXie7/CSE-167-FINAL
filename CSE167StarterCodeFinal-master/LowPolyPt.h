/**
 * File Name:     lowPolyPt.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   Header for low poly point
 */
#ifndef LOWPOLYPT_H
#define LOWPOLYPT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <functional>

#include <stdio.h>
#include <math.h>
#include <iostream>

// pair of vert norm
struct vertNormColorPair {
  glm::vec3 vertex;
  glm::vec3 norm;
  glm::vec3 color;
  int used;
};

struct LowPolyPt {
  // static color interpolate function
  static std::function<glm::vec3(glm::vec3, float)> colorInterpolateFunc;
  static float radius;
  void static setColorInterpolateFunc(std::function<glm::vec3(glm::vec3, float)>);
  void static setRadiusScale(float);

  struct vertNormColorPair pts[6];

  LowPolyPt(glm::vec3);
  int setNorm(glm::vec3);
  glm::vec3 getCoord();
};

#endif


