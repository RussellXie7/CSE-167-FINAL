#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include <math.h>
#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <glm/gtx/transform.hpp>

class boundingBox
{
public:
	boundingBox();
	boundingBox(glm::vec3 center, float width, float height, float depth);
	void drawBbox();
	int detectCollision(boundingBox* another);
	void setCenter(glm::vec3 center);
	void setParameter(float width, float height, float depth);
	float width, height, depth;
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	glm::vec3 center;
	int detectCollisionHeight();
};

#endif