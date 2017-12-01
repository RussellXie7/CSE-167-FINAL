#include "Curve.h"

vec3 pre_point;


Curve::Curve(vec3 p0, vec3 p1, vec3 p2, vec3 p3, int num_segments, int is_accel)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->num_curve_points = num_segments + 1;
	this->is_accel = is_accel;
	CreateCurvePoints();
	InitializeBuffer();
}

Curve::~Curve(){}



void Curve::InitializeBuffer() {
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// GL_ARRAY_BUFFER is an array containing relevant data to vertices, normals, colors, etc.
	// Pass in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

}
void Curve::draw(glm::mat4 C) {}
void Curve::draw(glm::mat4 C, GLint shaderProgram){

	// Calculate the combination of the model and view (camera inverse) matrices
	// Warning: could be wrong and should remove Window::V
	glm::mat4 modelview = Window::V * C;

	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);

	glUniform1i(glGetUniformLocation(shaderProgram, "is_accel"), 0);

	// Now draw the object. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}


void Curve::update(){}


float C0(float t) {
	float diff = 1 - t;
	return pow(diff,3);
}

float C1(float t) {
	float diff = 1 - t;
	return (3 * t * pow(diff, 2));
}

float C2(float t) {
	float diff = 1 - t;
	return ( 3 * pow(t, 2) * diff);
}

float C3(float t) {
	return pow(t, 3);
}

void Curve::CreateCurvePoints() {
	max_height = -9999999;

	for (int i = 0; i < num_curve_points; i++) {
		float t = i / ((float)num_curve_points-1);

		// calculate points c0p0 ..... 
		vec3 curr_point = C0(t) * p0 + C1(t) * p1 + C2(t) * p2 + C3(t) * p3;
		//cout << to_string(t) << endl;
		vertices.push_back(curr_point);

		if (max_height < curr_point.y) {
			max_height = curr_point.y;
			mountPoint = curr_point;
			mount_t = t;
		}

		if (i == 0) {
			pre_point = curr_point;
			length = 0;
		}
		else {
			length += glm::length(curr_point - pre_point);
			pre_point = curr_point;
		}
	}

	//cout << "vertices: " << vertices.size() << endl;
}

vec3 Curve::calcPoint(float t) {
	return (C0(t) * p0 + C1(t) * p1 + C2(t) * p2 + C3(t) * p3);
}

void Curve::UpdateCurvePoints(int index, vec3 value) {
	max_height = -99999999;

	switch (index) {
		case 0: p0 = value; break;
		case 1: p1 = value; break;
		case 2: p2 = value; break;
		case 3: p3 = value; break;
		default: printf("index passed into updateCurvePoints is wrong \n");
	}

	for (int i = 0; i < num_curve_points; i++) {
		float t = i / ((float)num_curve_points - 1);

		// calculate points c0p0 ..... 
		vec3 curr_point = C0(t) * p0 + C1(t) * p1 + C2(t) * p2 + C3(t) * p3;

		// update verticies
		vertices[i] = curr_point;

		if (max_height < curr_point.y) {
			max_height = curr_point.y;
			mountPoint = curr_point;
			mount_t = t;
		}


		if (i == 0) {
			pre_point = curr_point;
			length = 0;
		}
		else {
			length += glm::length(curr_point - pre_point);
			pre_point = curr_point;
		}
	}

	InitializeBuffer();
}

