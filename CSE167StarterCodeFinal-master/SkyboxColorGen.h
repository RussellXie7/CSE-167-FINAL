/**
 * File Name:     SkyboxColorGen.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   group of function that generate skybox color based on height
 */
#ifndef SKYBOXCOLORGEN_H
#define SKYBOXCOLORGEN_H

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

class SkyboxColorGen {
public:
  static glm::vec3 getColor(glm::vec3, float);
private:
  static std::default_random_engine gen;
  static std::normal_distribution<float> dist;
  static glm::vec3 colors[4];
  static float scale;
  static glm::vec3 sun;
};

#endif






