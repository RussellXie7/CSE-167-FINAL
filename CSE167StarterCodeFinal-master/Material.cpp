#include "Material.h"


Material::Material() {
	make_both();
}

Material::~Material() {

}

Material::Material(int mode) {
	if (mode == 0) make_only_shiny();
	if (mode == 1) make_only_diffuse();
	if (mode == 2) make_both();
}

void Material::make_only_shiny() {
	obj_mode = 2;
	color_ambi = glm::vec3(0.1,0.01,0.01);
	color_diff = glm::vec3(0, 0, 0);
	color_spec = glm::vec3(1, 200/(float)255, 0);

	shiness = 0.9;
}

void Material::make_only_diffuse() {
	obj_mode = 3;
	color_ambi = glm::vec3(0.1,0.01,0.01);
	color_diff = glm::vec3(0.9,0.9,0.9);
	color_spec = glm::vec3(0,0,0);

	shiness = 0;
}


void Material::make_both() {

	color_ambi = glm::vec3(0.1,0.01,0.01);
	color_diff = glm::vec3(0.9,0.9,0.9);
	color_spec = glm::vec3(0.5,0,0);

	shiness = 0.5;
}

void Material::make_pointLight_ball(glm::vec3 lightColor) {
	obj_mode = 1;

	color_ambi = lightColor;
	color_spec = glm::vec3(0, 0, 0);
	color_diff = glm::vec3(0, 0, 0);

	shiness = 0;
}


void Material::draw(GLuint shaderProgram) {
	glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), color_diff.x, color_diff.y, color_diff.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), color_spec.x, color_spec.y, color_spec.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), color_ambi.x, color_ambi.y, color_ambi.z);

	glUniform1f(glGetUniformLocation(shaderProgram, "material.shiness"), shiness);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.obj_mode"), obj_mode);
}