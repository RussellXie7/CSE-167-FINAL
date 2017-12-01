#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class BoundingSphere
{
private:
	std::vector<unsigned int> indices;

public:
	glm::mat4 M;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	BoundingSphere(const char* filepath);

	float currScale;

	void parse(const char* filepath);

	void draw(glm::mat4 C);
	void update();

	float ConvertColor(float);

	void Scale(float);

};

#endif