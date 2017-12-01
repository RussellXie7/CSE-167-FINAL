#ifndef OBJ_H
#define OBJ_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <iostream>
#include <fstream>
#include "glm/ext.hpp"
#include <vector>
#include "Group.h"

class OBJObject : public Node
{
private:
	std::vector<unsigned int> indices;

public:
	OBJObject(const char* filepath);
	~OBJObject();

	// function derived from Node
	void draw(glm::mat4 C);
	void update();  

	void parse(const char* filepath);

	// These variables are needed for the shader program
	GLuint VBO, VBO2, VAO, EBO;
	GLuint uProjection, uView, uMode;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	// convert the normal value to RGB values
	float ConvertColor(float);
};

#endif