#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <list>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <time.h>
#include <math.h>
#include <stdlib.h>

using namespace std;
using namespace glm;


class Node {

public:
	virtual void draw(glm::mat4 C)=0;
	virtual void update()=0;
};

#endif