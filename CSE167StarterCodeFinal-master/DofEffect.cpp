// This is the final version
/**

Usage: include "DofEffect.h" at the top of your Window.cpp, then, make sure you have shader.h:

1. Declaration: copy this:

#define SCENE_VERTEX_SHADER_PATH "../Shaders/bokeh.vert"
#define SCENE_FRAGMENT_SHADER_PATH "../Shaders/bokeh.frag"
GLint screenShaderProgram;
bool isDof = true;
DofEffect* dof_effect;

2. in your initialization: copy this:

screenShaderProgram = LoadShaders(SCENE_VERTEX_SHADER_PATH, SCENE_FRAGMENT_SHADER_PATH);
dof_effect = new DofEffect(screenShaderProgram);

3. in your display_callback or draw function,
   3.1. in the beginning of where you want to post process, most likely in the first line copy:

	if (isDof) {
	dof_effect->bindFrameBuffer();
	}

   3.2. in the end, most likely right before swapbuffers copy:

   // func(bokeh_shader, aperture, focus, maxBlur)
   if (isDof) {
   dof_effect->dof_post_processing(screenShaderProgram);
   }

4. at your key callback:


	if (key == GLFW_KEY_P) {
	isDof = !isDof;
	}

	if (key == GLFW_KEY_U) {
	dof_effect->increase_focus();
	}

	if (key == GLFW_KEY_I) {
	dof_effect->decrease_focus();
	}

 */

#include "DofEffect.h"
#include "Window.h"

#include <iostream>


using namespace std;
using namespace glm;

GLuint frameBuffer;
GLuint colormap;
GLuint depthmap;
GLuint dofVAO, dofVBO;

// Quad vertices
GLfloat quadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

// need to be called after the framebuffer has been created
void createColorDepthMap() {
	glGenTextures(1, &depthmap);
	glGenTextures(1, &colormap);

	// DEPTH
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Window::width, Window::height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0
	);

	// COLOR
	glBindTexture(GL_TEXTURE_2D, colormap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, Window::width, Window::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
	);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colormap, 0
	);


}


void createFrameBuffer() {
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

DofEffect::DofEffect(GLuint screenShaderProgram){

	aperture = 2.0f;
	focus = 1.0f;
	max_blur = 1.0f;

	// create the frame buffer and depth buffer color buffer
	glGenVertexArrays(1, &dofVAO);
	glGenBuffers(1, &dofVBO);

	glBindVertexArray(dofVAO);
	glBindBuffer(GL_ARRAY_BUFFER, dofVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(screenShaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(screenShaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// create them
	createFrameBuffer();
	createColorDepthMap();

	// check if it is completed
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err == GL_FRAMEBUFFER_COMPLETE) {
		cout << "frame buffer is completed" << endl;
	}

}

DofEffect::~DofEffect(){}

void DOF_Postprocessing(GLuint screenShaderProgram, float aperture, float focus, float maxBlur) {

	glUseProgram(screenShaderProgram);
	glBindVertexArray(dofVAO);

	glUniform1f(glGetUniformLocation(screenShaderProgram, "maxBlur"), maxBlur);
	glUniform1f(glGetUniformLocation(screenShaderProgram, "aperture"), aperture);
	glUniform1f(glGetUniformLocation(screenShaderProgram, "focus"), focus);
	glUniform1f(glGetUniformLocation(screenShaderProgram, "aspect"), Window::width / (float)Window::height);

	//cout << "the sizes are " << Window::width << " and " << Window::height << " and " << Window::width / (float)Window::height << endl;

	// bindColormap
	glUniform1i(glGetUniformLocation(screenShaderProgram, "texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colormap);

	// bind ddepthmap
	glUniform1i(glGetUniformLocation(screenShaderProgram, "tDepth"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthmap);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

}

void DofEffect::bindFrameBuffer() {
	// use the customized framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void DofEffect::dof_post_processing(GLuint screenShaderProgram) {
	// unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// DOF post processing
	DOF_Postprocessing(screenShaderProgram, aperture, focus, max_blur);
}

// controls
void DofEffect::increase_focus() {
	// cap to 1
	float temp = focus + 0.001f;

	focus = temp;
	cout << "focus is " << focus << endl;
}

void DofEffect::decrease_focus() {
	// cap to 1
	float temp = focus - 0.001f;
	focus = temp;

	cout << "focus is " << focus << endl;
}

void DofEffect::reset_focus() {
	focus = 1.0f;
}

void DofEffect::increase_aperture() {
	aperture += 0.1f;

	cout << "aperture is " << aperture << endl;
}

void DofEffect::decrease_aperture() {
	aperture -= 0.1f;
	cout << "aperture is " << aperture << endl;
}



