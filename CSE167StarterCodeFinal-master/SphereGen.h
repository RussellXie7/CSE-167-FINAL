/**
 * File Name:     SphereGen.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   group of function that implement procedual generation terrain
 */
#ifndef SPHEREGEN_H
#define SPHEREGEN_H

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
#include <random>

#include <stdio.h>
#include <math.h>
#include <iostream>

class SphereGen {
public:
  static glm::vec3 getHeightUpper(float x, float z, float radius);
  static glm::vec3 getHeightLower(float x, float z, float radius);
private:
  static std::default_random_engine gen;
  static std::normal_distribution<float> dist;
};

#endif




