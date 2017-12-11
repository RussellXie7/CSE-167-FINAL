#ifndef SKYBOX_H
#define SKYBOX_H

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
#include <string>
#include <array>
#include "Window.h"

#include <stdio.h>
#include <math.h>
#include <iostream>

#define SIZE 2.0f


class Skybox {

public:

	Skybox(std::vector<std::string>);
  ~Skybox();

  glm::mat4 toWorld;

  GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
  GLuint textureID;

  void loadTexture(std::vector<std::string>);
  void setupSkybox();
	void draw(GLuint);
};

const GLfloat vertices[8][3] = {
	// "Front" vertices
	{-SIZE, -SIZE, SIZE}, {SIZE, -SIZE, SIZE}, {SIZE,  SIZE,  SIZE}, {-SIZE,  SIZE,  SIZE},
	// "Back" vertices
	{-SIZE, -SIZE, -SIZE}, {SIZE, -SIZE, -SIZE}, {SIZE,  SIZE, -SIZE}, {-SIZE,  SIZE, -SIZE}
};

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
const GLuint indices[6][6] = {
	// Front face
	{0, 2, 1, 3, 2, 0},
	// Top face
	{1, 6, 5, 2, 6, 1},
	// Back face
	{7, 5, 6, 4, 5, 7},
	// Bottom face
	{4, 3, 0, 7, 3, 4},
	// Left face
	{4, 1, 5, 0, 1, 4},
	// Right face
	{3, 6, 2, 7, 6, 3}
};

#endif

