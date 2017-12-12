/**
 * File Name:     LowPolyOBJ.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   load obj to have low poly effect
 */
#ifndef LOWPOLYOBJ_H
#define LOWPOLYOBJ_H
#define _CRT_SECURE_NO_WARNINGS
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
#include <fstream>

/* need Window::V, Window::P */
#include "LowPolyPt.h"
#include "Window.h"
#include "boundingBox.h"

struct LowPolyOBJ {
  std::function<glm::vec3(glm::vec3, float)> colorFunc;

  int inOrOut;              // structure have normal vec point in or out, 1 means out, -1 mean in
  glm::mat4 toWorld;        // generated vertex should roughly be bounded in box (2 * radius) ^ 3
  GLuint VAO, VBO, EBO;

  std::vector<vertNormColorPair> vertices;
  std::vector<int> faces;
  boundingBox* box;
  int check = 0;
  glm::vec3 center;

  LowPolyOBJ(int, char *, std::function<glm::vec3(glm::vec3, float)>); 

  void parse(char *);
  void bindVAOVBOEBO();
  void draw(GLuint, int);
  void translate(float locX, float locY, float locZ);
  void orbitY(int counter);
  void scale(float size);
  void orbittranslate(float locX, float locY, float locZ, float ang);
};
#endif

