#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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

#include <vector>
#include "Window.h"

#include "Group.h"

class ControlPoint : public Node
{
public:
	ControlPoint();
	~ControlPoint();

	// function derived from Node
	void draw(glm::mat4 C);
	void update();  

	std::vector<glm::vec3> vertices;

	// These variables are needed for the shader program
	GLuint VAO,VBO;
	GLuint uProjection, uModelview, uMode;

};

#endif