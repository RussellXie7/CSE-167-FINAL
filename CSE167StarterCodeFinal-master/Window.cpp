#ifndef STB_IMAGE_IMPLEMENTATION    
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"
#endif

#include "Window.h"
#include <math.h>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

#include "shader.h"
#include "SkyboxColorGen.h"
#include "TerrainColorGen.h"
#include "SphereGen.h"
#include "TerrainGen.h"
#include "Terrain.h"
#include "LowPolyOBJ.h"
#include "LowPolyWater.h"
#include "DofEffect.h"

using namespace std;

#pragma region Old Declarations

const char* window_title = "GLFW Starter Project";

GLint skyboxShaderProgram;

GLuint * Window::shader;
int Window::shaderNum = 0;

Terrain * island;
LowPolyWater * water;

// On some systems you need to change this to the absolute path
const char * shaderPath[] = {
  "../LowPoly.vert",
  "../LowPoly.frag",
  "../Water.vert",
  "../Water.frag"
};

// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 60.0f, 60.0f);		// e  | Position of camera
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

#pragma endregion

#define SCENE_VERTEX_SHADER_PATH "../Shaders/bokeh.vert"
#define SCENE_FRAGMENT_SHADER_PATH "../Shaders/bokeh.frag"
GLint screenShaderProgram;
bool isDof = true;
DofEffect* dof_effect;

void Window::initialize_objects()
{
  shaderNum = sizeof(shaderPath) / sizeof(char *) / 2;
  shader = (GLuint *) malloc(shaderNum * sizeof(GLuint));
  for (int i = 0; i < shaderNum; i++) {
    shader[i] = LoadShaders(shaderPath[2 * i], shaderPath[2 * i + 1]);
  }

  island = new Terrain(30, 1, 1, TerrainGen::getHeight, 
      SphereGen::getHeightLower, TerrainColorGen::getColor);
  water = new LowPolyWater(30, 0.0f, 1); 
  std::cout << water->faces.size() << "\n";
  std::cout << "\n";

  screenShaderProgram = LoadShaders(SCENE_VERTEX_SHADER_PATH, SCENE_FRAGMENT_SHADER_PATH);
  dof_effect = new DofEffect(screenShaderProgram);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	//if (toDisplay != NULL) delete(toDisplay);
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

glm::vec3 Window::trackBallMapping(double x, double y)
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (float)(2.0 * x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (float)(height - 2.0 * y) / height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0f) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001f - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v = normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (old_xpos == -99999) old_xpos = xpos;
	if (old_ypos == -99999) old_ypos = ypos;
	
	glm::vec3 prev_pos = trackBallMapping(old_xpos,old_ypos);
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec3 curr_pos = trackBallMapping(xpos, ypos);

	if (left_mouse_on) {
		// do selectionbuffer
		// CameraOrbit(float(xpos - old_xpos), float(ypos - old_ypos));
		float rotation_angle;
		// dot product and then divide by the mutiple of them to get the ratation angle
		rotation_angle = glm::acos((glm::dot(prev_pos, curr_pos) / (glm::length(prev_pos) * glm::length(curr_pos))));
		float cos = glm::dot(prev_pos, curr_pos) / (glm::length(prev_pos) * glm::length(curr_pos));
		if (cos > 1)
		{
			cos = 1;
			rotation_angle = acos(cos);
		}
		glm::vec3 rotation_axis;
		// cross product to get the rotation axis
		rotation_axis = glm::cross(prev_pos, curr_pos);
		// if too less time, do not do anything
		if ((glm::length(curr_pos - prev_pos) > 0.001)) {
			float x_diff = xpos - old_xpos;
			float y_diff = ypos - old_ypos;

			if (cam_pos.z < 0) y_diff = -y_diff;

			float off = 0.3f;
			cam_pos = glm::rotate(glm::mat4(), (float)(off * -(x_diff) / 180.0f) * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f);
			cam_pos = glm::rotate(glm::mat4(), (float)(off * -(y_diff) / 180.0f) * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f);
			cam_up = glm::rotate(glm::mat4(), (float)(off * (x_diff) / 180.0f) * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_up, 1.0f);
			cam_look_at = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cam_pos);
			P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
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

	water->reflection.resize();
	water->refraction.resize();

}

void Window::idle_callback()
{
	// if we updating the sphere location
}

void Window::display_callback(GLFWwindow* window)
{

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// prepare water FBO
	water->prepTexture();
	
	if (isDof) {
		dof_effect->bindFrameBuffer();
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// draw with priority 0
	render(0);

	// func(bokeh_shader, aperture, focus, maxBlur)
	if (isDof) {
		dof_effect->dof_post_processing(screenShaderProgram);
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::render(unsigned int priority) {
  island->draw(shader[0], priority);
  water->draw(shader[1], priority);
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
		
	if (key == GLFW_KEY_P) {
	isDof = !isDof;
	}

	if (key == GLFW_KEY_U) {
	dof_effect->increase_focus();
	}

	if (key == GLFW_KEY_I) {
	dof_effect->decrease_focus();
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




