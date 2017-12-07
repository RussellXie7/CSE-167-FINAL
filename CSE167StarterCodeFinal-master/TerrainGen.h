/**
 * File Name:     TerrainGen.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   group of function that implement procedual generation terrain
 */
#ifndef TERRAINGEN_H
#define TERRAINGEN_H

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

#define OCTAVES 3
#define ROUGHNESS 0.3f

class TerrainGen {
public:
  static glm::vec3 getHeight(float x, float z, float radius);
private:
  static std::default_random_engine gen;
  static std::default_random_engine genDist;
  static std::normal_distribution<float> dist;

  static float getInterpolatedNoise(float x, float z, float radius);
  static float interpolate(float x, float y, float alpha);
  static float getSmoothNoise(int x, int z, float radius);
  static float getNoise(int x, int z, float radius);
};

#endif



