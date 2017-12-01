#include "BoundingSphere.h"

#include <iostream>
#include <fstream>
#include "glm/ext.hpp"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

BoundingSphere::BoundingSphere(const char *filepath)
{
	M = glm::mat4(1.0f);
	parse(filepath);
}

void BoundingSphere::parse(const char *filepath)
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data

	// Read the Obj file
	FILE* fp;             // file pointer
	float x, y, z, xn, yn, zn;        // vertex coordinates
	float r, g, b;        // vertex color
	int c1, c2;           // characters read from file

	fp = fopen(filepath, "rb");          // make the file name configurable so you can load other files
	if (fp == NULL) { cerr << "error loading file" << endl; exit(-1); }

	while (true) {
		c1 = fgetc(fp);
		c2 = fgetc(fp);

		if (c1 == EOF || c2 == EOF) { break; }

		if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, "%f %f %f", &xn, &yn, &zn);
			normals.push_back(glm::vec3(xn, yn, zn));
		}

		else if (c1 == 'v' && c2 == ' ') {
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
		}
	}

	currScale = 1;

	printf("The number of vertices are %d, and normal is %d\n", vertices.size(), normals.size());
}

float BoundingSphere::ConvertColor(float normal) {

	float returnVal = normal;

	returnVal = (float)(returnVal + 1) / (float)2.0;

	return returnVal;
}


void BoundingSphere::Scale(float scaleInc) {
	if (scaleInc > 0) {
		//cout << glm::to_string(M) << endl;
		glm::vec3 scaleVector = glm::vec3(2, 2, 2);
		this->M = glm::scale(M, scaleVector);
		//cout << glm::to_string(M) << endl;
	}
	if (scaleInc < 0) {
		glm::vec3 scaleVector = glm::vec3(0.5, 0.5, 0.5);
		this->M = glm::scale(M, scaleVector);
	}
}

void BoundingSphere::draw(glm::mat4 C)
{
	this->M = C;

	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the M matrix
	glPushMatrix();
	glMultMatrixf(&(M[0][0]));

	glBegin(GL_POINTS);

	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		// color
		normals[i] = glm::normalize(normals[i]);
		//glNormal3f(normals[i].x, normals[i].y, normals[i].z);
		glColor3f(ConvertColor(normals[i].x), ConvertColor(normals[i].y), ConvertColor(normals[i].z));

		// position
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void BoundingSphere::update() {}
