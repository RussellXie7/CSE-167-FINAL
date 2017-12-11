#ifndef _DOF_EFFECT_
#define _DOF_EFFECT_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "glm/ext.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <math.h>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <random>


class DofEffect{

public:
	DofEffect(GLuint);
	~DofEffect();

	/// need to be called at the front of the thing that you want to post processing
	void bindFrameBuffer();
	/// better be called before swapping buffer
	void dof_post_processing(GLuint screenShaderProgram);

	// controls
	void increase_aperture();
	void decrease_aperture();
	void reset_aperture();

	void increase_focus();
	void decrease_focus();
	void reset_focus();

	void increase_blur();
	void decrease_blur();
	void reset_blur();


	float aperture, focus, max_blur;
};

#endif