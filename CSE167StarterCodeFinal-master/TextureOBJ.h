/**
 * File Name:     TextureOBJ.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   load obj to with texture
 */
#ifndef TEXTUREOBJ_H
#define TEXTUREOBJ_H

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
#include "Window.h"

struct vertTexPair {
  glm::vec3 vertex;
  glm::vec2 tex;

  vertTexPair(glm::vec3, glm::vec2);
};

struct TextureOBJ {

  glm::mat4 toWorld;        // generated vertex should roughly be bounded in box (2 * radius) ^ 3
  GLuint VAO, VBO, EBO;
  GLuint texture;

  std::vector<vertTexPair> vertices;
  std::vector<int> faces;

  TextureOBJ(char *, char *);

  void parse(char *);
  void loadTex(char *);
  void bindVAOVBOEBO();
  void draw(GLuint);
};

#endif





