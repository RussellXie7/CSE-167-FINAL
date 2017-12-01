#include "Light.h"

Light::Light()
{
	// we use desert light pattern by default
	constant = 0.1f;
	linear = 0.09f;
	quadratic = 0.032f;
	
	// default it is directional light
	make_directional_light();
}
Light::Light(int mode)
{
	// we use desert light pattern by default
	constant = 0.1f;
	linear = 0.09f;
	quadratic = 0.032f;

	if (mode == 2) make_point_light();
	else if (mode == 3) make_spot_light();
	else make_directional_light();
}


Light::~Light()
{
}

void Light::draw(GLuint shaderProgram) {
	glUniform1i(glGetUniformLocation(shaderProgram, "light.light_mode"), light_mode);
	glUniform3f(glGetUniformLocation(shaderProgram, "light.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "light.specular"), specular.x, specular.y, specular.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "light.position"), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "light.direction"), direction.x, direction.y, direction.z);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.constant"), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.linear"), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.quadratic"), quadratic);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.cutoff_angle"), cutoff_angle);
	glUniform1f(glGetUniformLocation(shaderProgram, "light.exponent"), exponent);
}


void Light::make_directional_light() {
	light_mode = 0;
	direction = glm::vec3(-2, -10, 0);

	ambient = glm::vec3(1, 1, 1);
	diffuse = glm::vec3(1, 1, 1);
	specular = glm::vec3(1, 1, 1);
}

void Light::make_point_light() {
	light_mode = 1;
	constant = 0.3;

	ambient = glm::vec3(253.0f/255.0f, 184 / 255.0f, 19 / 255.0f);
	diffuse = glm::vec3(253.0f / 255.0f, 184 / 255.0f, 19 / 255.0f);
	specular = glm::vec3(253.0f / 255.0f, 184 / 255.0f, 19 / 255.0f);

	// default position at 0,0,0
	position = glm::vec3(0, 0, 0);
}

void Light::make_spot_light() {
	light_mode = 2;

	ambient = glm::vec3(1, 1, 1);
	diffuse = glm::vec3(1, 1, 1);
	specular = glm::vec3(1, 1, 1);

	// default position at 0,0,0
	position = glm::vec3(0, 0, 0);
	direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	cutoff_angle = 10;
	exponent = 1.5f;
}