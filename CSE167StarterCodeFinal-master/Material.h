#ifndef MATERIAL_H
#define MATERIAL_H

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

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Material 
{
public:
	Material();
	Material(int);
	~Material();

	int obj_mode = 0;

	glm::vec3 color_diff = glm::vec3(0,0,0);
	glm::vec3 color_spec = glm::vec3(0, 0, 0);
	glm::vec3 color_ambi = glm::vec3(0, 0, 0);
	GLfloat shiness = 0;

	// for materials
    void make_only_shiny();
	void make_only_diffuse();
	void make_both();
	void make_pointLight_ball(glm::vec3 lightColor);

	void draw(GLuint);
};


#endif