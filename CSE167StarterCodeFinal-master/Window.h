#pragma comment(lib, "irrKlang.lib")

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "glm/ext.hpp" 
#include <stdlib.h>
#include "./GLFWStarterProject/include/irrKlang.h"
#include <string>

class Window
{

public:
	static glm::vec3 cam_pos, cam_look_at, cam_up;

	static int width;
	static int height;
	static bool left_mouse_on;
	static bool right_mouse_on;
	static double old_xpos;
	static double old_ypos;

  static GLuint * shader;
  static int shaderNum;

	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
  static void render(unsigned int);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	// Mouse control callbacks
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// camera control function
	static void CameraOrbit(float, float);
	static void CameraTranslate(glm::vec3);

	static glm::vec3 trackBallMapping(double x, double y);

};
#endif