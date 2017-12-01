#ifndef LIGHT_H
#define LIGHT_H

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


class Light
{
public:
	Light();
	Light(int mode);
	~Light();

	int light_mode = -1;

	glm::vec3 color = glm::vec3(0,0,0);

	glm::vec3 ambient = glm::vec3(0, 0, 0);
	glm::vec3 diffuse = glm::vec3(0, 0, 0);
	glm::vec3 specular = glm::vec3(0, 0, 0);

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 direction = glm::vec3(0, 0, 0);

	float constant = 0;
	float linear = 0;
	float quadratic = 0;

	float cutoff_angle = 0;
	float exponent = 0;

	void draw(GLuint);
	void make_directional_light();
	void make_point_light();
	void make_spot_light();
	
};
#endif