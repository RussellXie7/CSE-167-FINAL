#include "OBJObject.h"
#include "Window.h"

using namespace std;
using namespace glm;


OBJObject::OBJObject(const char *filepath)
{
	parse(filepath);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// GL_ARRAY_BUFFER is an array containing relevant data to vertices, normals, colors, etc.
	// Pass in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// Pass in VBO2
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	// Pass in EBO  
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

#pragma region Old Function for parsing, coloring

void OBJObject::parse(const char *filepath)
{

	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data

	// Read the Obj file
	FILE* fp;             // file pointer
	float x, y, z, xn, yn, zn;        // vertex coordinates
	float r, g, b;        // vertex color
	int c1, c2;           // characters read from file
	int v1, v2, v3, n1, n2, n3; // indices for v

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

		else if (c1 == '#' || c2 == '#') {
			// skip line
			fscanf(fp, "\n");
		}
		else if ((c1 == 'f' && c2 == ' ') || (c1 == '\n' && c2 == 'f')) {
			fscanf(fp, "%u//%u %u//%u %u//%u", &v1, &n1, &v2, &n2, &v3, &n3);
			indices.push_back(v1 - 1);
			indices.push_back(v2 - 1);
			indices.push_back(v3 - 1);

		}
	}

	printf("The number of vertices are %d, and normal is %d\n, and indices is %d\n", vertices.size(), normals.size(), indices.size());
}

// Convert the Normal values to RGB Color values
float OBJObject::ConvertColor(float normal) {

	float returnVal = normal;

	returnVal = (float)(returnVal + 1) / (float)2.0;

	return returnVal;
}
#pragma endregion


void OBJObject::draw(glm::mat4 C)
{
	GLint shaderProgram = Window::reflectionShaderProgram;

	// Calculate the combination of the model and view (camera inverse) matrices
	// Warning: could be wrong and should remove Window::V
	glm::mat4 modelview = Window::V * C;

	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);
	glUniform3f(glGetUniformLocation(shaderProgram, "cameraPos"), Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
	// Now draw the object. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Window::cubemapTexture);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}


void OBJObject::update()
{
	
}


