#ifndef CURVE_H
#define CURVE_H

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

class Curve : public Node
{
public:
	Curve(vec3, vec3, vec3, vec3, int, int);
	~Curve();

	// function derived from Node
	void draw(glm::mat4 C);
	void draw(glm::mat4 C, GLint shaderProgram);
	void update();  
	vec3 calcPoint(float);
	
	

	// Fill in the vertices for this cubic curve
	void CreateCurvePoints();
	void UpdateCurvePoints(int index, vec3 value);
	void InitializeBuffer();


	std::vector<glm::vec3> vertices;

	// These variables are needed for the shader program
	GLuint VBO, VAO;
	GLuint uProjection, uModelview, uMode;
	vec3 p0, p1, p2, p3;
	int num_curve_points;
	float length;
	float max_height;
	vec3 mountPoint;
	float mount_t;
	int is_accel;

};

#endif