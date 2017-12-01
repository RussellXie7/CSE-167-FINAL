
#ifndef STB_IMAGE_IMPLEMENTATION    
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"
#endif

#include "window.h"
#include <math.h>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

#include "shader.h"
#include "Cube.h"
#include "OBJObject.h"

using namespace std;

#pragma region Old Declarations

// replace the following images with out new skybox images
vector<std::string> faces
{
	"glacier_ft.tga",
	"glacier_bk.tga",
	"glacier_up.tga",
	"glacier_dn.tga",
	"glacier_rt.tga",
	"glacier_lf.tga"
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};


const char* window_title = "GLFW Starter Project";


GLint skyboxShaderProgram;

GLuint skyboxVAO, skyboxVBO;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../Shaders/curveShader.vert"
#define FRAGMENT_SHADER_PATH "../Shaders/curveShader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "../Shaders/skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../Shaders/skyboxShader.frag"

#define ANCHOR_VERTEX_SHADER_PATH "../Shaders/anchorShader.vert"
#define ANCHOR_FRAGMENT_SHADER_PATH "../Shaders/anchorShader.frag"

#define CONTROL_VERTEX_SHADER_PATH "../Shaders/controlShader.vert"
#define CONTROL_FRAGMENT_SHADER_PATH "../Shaders/controlShader.frag"

#define TANGENT_VERTEX_SHADER_PATH "../Shaders/tangentShader.vert"
#define TANGENT_FRAGMENT_SHADER_PATH "../Shaders/tangentShader.frag"

#define SELECTION_VERTEX_SHADER_PATH "../Shaders/selectionShader.vert"
#define SELECTION_FRAGMENT_SHADER_PATH "../Shaders/selectionShader.frag"

#define REFLECTION_VERTEX_SHADER_PATH "../Shaders/reflectionShader.vert"
#define REFLECTION_FRAGMENT_SHADER_PATH "../Shaders/reflectionShader.frag"

// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 0.0f, 150.0f);		// e  | Position of camera
glm::vec3 Window::cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 Window::cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
bool Window::left_mouse_on;
bool Window::right_mouse_on;
double Window::old_xpos;
double Window::old_ypos;

glm::mat4 Window::P;
glm::mat4 Window::V;

GLint Window::shaderProgram;
GLint Window::anchorShaderProgram;
GLint Window::controlShaderProgram;
GLint Window::tangentShaderProgram;
GLint Window::selectionShaderProgram;
GLint Window::reflectionShaderProgram;

unsigned int Window::cubemapTexture;

#pragma endregion

bool hide_sky_box = false;


unsigned int loadCubemap(vector<std::string> faces);
void draw_skybox();


void Window::initialize_objects()
{

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	skyboxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	anchorShaderProgram = LoadShaders(ANCHOR_VERTEX_SHADER_PATH, ANCHOR_FRAGMENT_SHADER_PATH);
	controlShaderProgram = LoadShaders(CONTROL_VERTEX_SHADER_PATH, CONTROL_FRAGMENT_SHADER_PATH);
	tangentShaderProgram = LoadShaders(TANGENT_VERTEX_SHADER_PATH, TANGENT_FRAGMENT_SHADER_PATH);
	selectionShaderProgram = LoadShaders(SELECTION_VERTEX_SHADER_PATH, SELECTION_FRAGMENT_SHADER_PATH);
	reflectionShaderProgram = LoadShaders(REFLECTION_VERTEX_SHADER_PATH, REFLECTION_FRAGMENT_SHADER_PATH);

	// load the skybox
	cubemapTexture = loadCubemap(faces);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#pragma region OldRegion

	// initialize mouse condition
	right_mouse_on = false;
	left_mouse_on = false;
	// initialize mouse position
	old_xpos = -99999;
	old_ypos = -99999;

	// Load the shader program. Make sure you have the correct filepath up top
	Window::shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

#pragma endregion

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	//if (toDisplay != NULL) delete(toDisplay);
	glDeleteProgram(Window::shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();

		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::CameraOrbit(float x_diff, float y_diff) {
	float offset = 0.3f;

	glm::vec4 cam_pos_vec4 = glm::vec4(cam_pos, 1);
	glm::vec4 cam_up_vec4 = glm::vec4(cam_up, 1);
	
	cam_pos_vec4 = glm::rotate(glm::mat4(1.0f), (offset * (-x_diff)) / 180.0f * glm::pi<float>(),
		glm::vec3(0.0f, 1.0f, 0.0f)) * cam_pos_vec4;

	cam_pos_vec4 = glm::rotate(glm::mat4(1.0f), (offset * (-y_diff)) / 180.0f * glm::pi<float>(),
		glm::vec3(1.0f, 0.0f, 0.0f)) * cam_pos_vec4;

	cam_pos = glm::vec3(cam_pos_vec4.x, cam_pos_vec4.y, cam_pos_vec4.z);

	cam_look_at = normalize(glm::vec3(0, 0, 0) - cam_pos);

	// update the up direction // uncomment following if you want to slant the camera.
	cam_up_vec4 = glm::rotate(glm::mat4(1.0f), (offset * x_diff) / 180.0f * glm::pi<float>(),
		glm::vec3(0.0f, 1.0f, 0.0f)) * cam_up_vec4;
	//cam_up_vec4 = glm::rotate(glm::mat4(1.0f), (offset * y_diff) / 180.0f * glm::pi<float>(),
	//	glm::vec3(1.0f, 0.0f, 0.0f)) * cam_up_vec4;
	cam_up = glm::vec3(cam_up_vec4.x, cam_up_vec4.y, cam_up_vec4.z);

	P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (old_xpos == -99999) old_xpos = xpos;
	if (old_ypos == -99999) old_ypos = ypos;
	
	if (left_mouse_on) {
		// do selectionbuffer
		CameraOrbit(float(xpos - old_xpos), float(ypos - old_ypos));
	}

	if (right_mouse_on) {
		// do something here
	
	}

	old_xpos = xpos;
	old_ypos = ypos;

}


void Window::CameraTranslate(glm::vec3 moveVector) {
	glm::vec4 cam_pos_vec4 = glm::vec4(cam_pos, 1);
	cam_pos_vec4 = glm::translate(glm::mat4(1.0f), moveVector) * cam_pos_vec4;
	cam_pos = glm::vec3(cam_pos_vec4.x, cam_pos_vec4.y, cam_pos_vec4.z);

	P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		right_mouse_on = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		right_mouse_on = false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		left_mouse_on = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		left_mouse_on = false;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (glm::length(glm::vec3(0, 0, 0) - cam_pos) > 20.0f || yoffset > 0)
		CameraTranslate(-yoffset * 10 * normalize(glm::vec3(0, 0, 0) - cam_pos));
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// if we updating the sphere location

}



void draw_skybox() {

	// use skybox shader????
	glUseProgram(skyboxShaderProgram);

	glDepthMask(GL_FALSE);
	glm::mat4 view = glm::mat4(glm::mat3(Window::V));
	// Now send these values to the shader program
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Window::cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// unbound skyboxVAO
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// draw skybox first
	if (!hide_sky_box) {
		draw_skybox();
	}

	// Use the shader of programID
	glUseProgram(Window::shaderProgram);


	// draw the actual geometry HERE!!!!!!!!!!


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);

}


unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	}

	// when that key is released
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_ESCAPE) {
			// do something here if necessary
		}
	}
}




