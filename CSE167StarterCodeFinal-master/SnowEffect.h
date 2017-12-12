#ifndef _SNOW_EFFECT_
#define _SNOW_EFFECT_

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


class SnowEffect{

public:
	SnowEffect();
	~SnowEffect();

	/// better be called before swapping buffer
	void drawSnow();

	// controls
	void increase_windSpeed();
	void decrease_windSpeed();

	void increase_windDirection();
	void decrease_windDirection();
};

#endif