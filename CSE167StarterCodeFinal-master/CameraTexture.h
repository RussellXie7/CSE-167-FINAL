/**
 * File Name:     CameraTexture.h
 * Author:        Chenxu Jiang
 * Date:          10/03/2017
 * Description:   take snapshot, store to a texture
 */
#ifndef CameraTexture_H
#define CameraTexture_H

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

struct CameraTexture {
  int priority;
  GLuint FBO;
  GLuint depthBuf;
  GLuint texture;

  CameraTexture(int);

  void takeSnapShot(glm::vec3, glm::vec3, glm::vec3, glm::vec4);
  void resize();
};

#endif





