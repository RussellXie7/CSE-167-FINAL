/**
 * File Name:     LowPolyWater.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   water 
 */
#ifndef LOWPOLYWATER_H
#define LOWPOLYWATER_H

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

/* need Window::V, Window::P */
#include "LowPolyPt.h"
#include "CameraTexture.h"
#include "Window.h"

struct WaterPt {
  glm::vec2 vertex;
  glm::vec4 neighbors;
  int used;
};

struct LowPolyWaterPt {
  WaterPt pts[6];

  LowPolyWaterPt(glm::vec2);
  int setNeighbor(glm::vec4);
  glm::vec2 getCoord();
};

struct LowPolyWater {
  int radius;               // squared number of vertex needed
  float waterLevel;           // water level
  float waveTime;
  int priority;
  glm::mat4 toWorld;        // vertex should roughly bounded in box (2 * radius) ^ 3
  GLuint VAO, VBO, EBO;

  CameraTexture refraction;
  CameraTexture reflection;

  std::vector<LowPolyWaterPt> vertices;
  std::vector<int> faces;

  LowPolyWater(int, float, int);

  void generateWaterVec();
  void generateFaces();
  void bindVAOVBOEBO();
  void prepTexture();
  void draw(GLuint, int);
};

#endif




