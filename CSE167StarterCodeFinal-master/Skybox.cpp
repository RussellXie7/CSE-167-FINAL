#include "Skybox.h"
#include "Window.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

Skybox::Skybox(std::vector<std::string> faces) {
  
  this->toWorld = glm::mat4(1.0f);

  loadTexture(faces);

  setupSkybox();
}

Skybox::~Skybox() {
  glDeleteVertexArrays(1, &VAO);

  glDeleteBuffers(1, &VBO);

  glDeleteBuffers(1, &EBO);
}

void Skybox::loadTexture(std::vector<std::string> faces) {
  
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
 		if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
          0, GL_RGB, width, height, 0,  GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    
		} else {
      std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::setupSkybox() {
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Skybox::draw(GLuint shaderProgram) {
  glm::mat4 view = glm::mat4(glm::mat3(Window::V));
  
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &(Window::P)[0][0]);

  glDepthMask(GL_FALSE);

  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glDepthMask(GL_TRUE);

}

