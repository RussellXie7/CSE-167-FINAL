/**
 * File Name:     Terrain.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   terrain is a round shape with 2 side
 */
#ifndef TERRAIN_H
#define TERRAIN_H

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
#include "Window.h"
#include "LowPolyPt.h"

struct Terrain {
  std::function<glm::vec3(float, float, float)> upFunc;
  std::function<glm::vec3(float, float, float)> downFunc;
  std::function<glm::vec3(glm::vec3, float)> colorFunc;

  int radius;               // squared number of vertex needed
  int inOrOut;              // structure have normal vec point in or out, 1 means out, -1 mean in
  int priority;
  glm::mat4 toWorld;        // generated vertex should roughly be bounded in box (2 * radius) ^ 3
  GLuint VAO, VBO, EBO;

  std::vector<LowPolyPt> vertices;
  std::vector<int> faces;
  std::vector<int> startIdx;
  std::vector<int> rowLen;

  Terrain(int, int, int, 
      std::function<glm::vec3(float, float, float)>, 
      std::function<glm::vec3(float, float, float)>, 
      std::function<glm::vec3(glm::vec3, float)>); 

  void generateTerrainVec();
  void generateFaces();
  void bindVAOVBOEBO();
  void draw(GLuint, int);
};

#endif



