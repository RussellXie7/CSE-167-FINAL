#include "LowPolyOBJ.h"

LowPolyOBJ::LowPolyOBJ(int inOrOut, 
                  char * filename,
                  std::function<glm::vec3(glm::vec3, float)> colorFunc) {
  
  this->inOrOut = inOrOut;
  this->colorFunc = colorFunc;  
  this->toWorld = glm::mat4(1.0f);
  float width = 0.708623f;
  float height = 0.499956f;
  float depth = 2.0f;
  center = glm::vec3(0.0f, 0.0f, 0.0f);
  box = new boundingBox(center, width, height, depth);
  parse(filename);
  bindVAOVBOEBO();
}

void LowPolyOBJ::parse(char * filename) {

  std::vector<int> faces_t;
  std::vector<glm::vec3> vertices_t;

  float x,y,z;      		// point coordinate
  float r,g,b;      		// point color
  float x_min, x_max, y_min, y_max, z_min, z_max;
  int o, p, j;
  // try open file
  std::ifstream infile(filename);

  // loop over file
  int lineNum = 1;
  std::string line;
	bool firstPoint = true;
  while (std::getline(infile, line)) {

    const char *buf = line.c_str();

    // skip any thing not starting with 'v'
    if (buf[0] != 'v' && buf[0] != 'f') {
			lineNum++;
      continue;
		}

    if (buf[0] == 'f') {
      GLuint n1, n2, n3, v1, v2, v3;
      int match = sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", 
          &v1, &o, &n1, &v2, &p, &n2, &v3, &j, &n3);
      
      if (match == 0) {
				std::cerr << buf << std::endl;
				std::cerr << x << " " << y << " " << z << std::endl;
        std::cerr << "Failed to parse line " << lineNum << std::endl;
    		lineNum++;
        continue;
      }

      faces_t.push_back(v1 - 1);
      faces_t.push_back(v2 - 1);
      faces_t.push_back(v3 - 1);
  
    // parse vertex 
    } else if (buf[0] == 'v' && buf[1] == ' ') {
      int match = sscanf(buf, "v %f %f %f\n", &x, &y, &z); 
      
      if (match == 0) 
				match = sscanf(buf, "v %f %f %f %f %f %f\n", &x, &y, &z, &r, &g, &b); 

			if (match == 0) {
				std::cerr << buf << std::endl;
				std::cerr << x << " " << y << " " << z << std::endl;
        std::cerr << "Failed to parse line " << lineNum << std::endl;
    		lineNum++;
        continue;
      } 

      vertices_t.push_back(glm::vec3(x, y, z)); 

			// get bounding box
			if (firstPoint) {
				x_min = x_max = x;
				y_min = y_max = y;
				z_min = z_max = z;		
				firstPoint = false;
			} else {
				x_min = x < x_min? x: x_min;
				x_max = x > x_max? x: x_max;
				y_min = y < y_min? y: y_min;
				y_max = y > y_max? y: y_max;
				z_min = z < z_min? z: z_min;
				z_max = z > z_max? z: z_max;
			}
    }

    lineNum++;
  }

	float transX = (x_max + x_min) / 2.0;
	float transY = (y_max + y_min) / 2.0;
	float transZ = (z_max + z_min) / 2.0;
	float scaleX = 2.0 / (x_max - x_min);
	float scaleY = 2.0 / (y_max - y_min);
	float scaleZ = 2.0 / (z_max - z_min);
	float scale = fminf(fminf(scaleX, scaleY), scaleZ);	

	//std::cout << (x_max - x_min) * scale << std::endl;
	//std::cout << (y_max - y_min) * scale << std::endl;
	//std::cout << (z_max - z_min) * scale << std::endl;
	//std::cout << "-------" << std::endl;
  firstPoint = true;
  LowPolyPt::setColorInterpolateFunc(colorFunc);
	for (unsigned int i = 0; i < vertices.size(); ++i) {
    vertices_t[i].x = (vertices_t[i].x - transX) * scale;
    vertices_t[i].y = (vertices_t[i].y - transY) * scale;
    vertices_t[i].z = (vertices_t[i].z - transZ) * scale;
  }

  for (unsigned int i = 0; i < faces_t.size(); i = i + 3) {
    int idx0 = faces_t[i];
    int idx1 = faces_t[i + 1];
    int idx2 = faces_t[i + 2];
    vertNormColorPair pt0, pt1, pt2;

    pt0.vertex = vertices_t[idx0];
    pt1.vertex = vertices_t[idx1];
    pt2.vertex = vertices_t[idx2];

    pt0.color = colorFunc(pt0.vertex, 2.0f);
    pt1.color = colorFunc(pt1.vertex, 2.0f);
    pt2.color = colorFunc(pt2.vertex, 2.0f);

    glm::vec3 dir0 = pt0.vertex - pt1.vertex;
    glm::vec3 dir1 = pt0.vertex - pt2.vertex;
    glm::vec3 norm = glm::normalize(glm::cross(dir0, dir1));

    // make sure norm point to direction in or out specified
    if (glm::dot(norm, pt0.vertex) * inOrOut < 0)
      norm = -norm;
    
    pt0.norm = norm;
    pt1.norm = norm;
    pt2.norm = norm;

    int currIdx = vertices.size();
    vertices.push_back(pt0);
    vertices.push_back(pt1);
    vertices.push_back(pt2);

    faces.push_back(currIdx);
    faces.push_back(currIdx + 1);
    faces.push_back(currIdx + 2);
  }
}

void LowPolyOBJ::bindVAOVBOEBO() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertNormColorPair),
      vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint),
      faces.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertNormColorPair), 
     (GLvoid *) offsetof(vertNormColorPair, vertex));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertNormColorPair), 
     (GLvoid *) offsetof(vertNormColorPair, norm));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertNormColorPair), 
     (GLvoid *) offsetof(vertNormColorPair, color));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void LowPolyOBJ::draw(GLuint shader) {
  glm::mat4 modelView = Window::V * toWorld;

  glUseProgram(shader);
  glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 
      1, GL_FALSE, &Window::P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader, "modelview"), 
      1, GL_FALSE, &modelView[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader, "model"),
      1, GL_FALSE, &toWorld[0][0]);
  glUniform3f(glGetUniformLocation(shader, "camPos"),
      Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
  glUniform1i(glGetUniformLocation(shader, "toon"),
	  1);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void LowPolyOBJ::drawTree(GLuint shader) {
	glm::mat4 modelView = Window::V * toWorld;

	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"),
		1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelview"),
		1, GL_FALSE, &modelView[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"),
		1, GL_FALSE, &toWorld[0][0]);
	glUniform3f(glGetUniformLocation(shader, "camPos"),
		Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
	glUniform1i(glGetUniformLocation(shader, "toon"),
		2);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void LowPolyOBJ::orbitY(int counter)
{
	float ang = 1.0f;
	if (counter == 0) 
	{
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		this->toWorld = rot * this->toWorld;
		return;
	}
	counter = counter % 4;
	if (counter == 0) 
	{
		glm::mat4 rotInv = glm::rotate(glm::mat4(1.0f), -ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		rotInv = glm::inverse(rotInv);
		this->toWorld = rotInv * this->toWorld;
		this->toWorld = rot * this->toWorld;
	}
	else 
	{
		glm::mat4 rotInv = glm::rotate(glm::mat4(1.0f), ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		rotInv = glm::inverse(rotInv);
		this->toWorld = rotInv * this->toWorld;
		this->toWorld = rot * this->toWorld; 
	}
}

void LowPolyOBJ::scale(float size)
{
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
	this->toWorld = scale * this->toWorld;
}

void LowPolyOBJ::translate(float locX, float locY, float locZ)
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(locX, locY, locZ));
	this->toWorld = trans * this->toWorld;
	this->center = this->toWorld * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	box->setCenter(center);
}

void LowPolyOBJ::orbittranslate(float locX, float locY, float locZ, float ang)
{
	//auto translateMat = glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
	//glm::mat4 translate = glm::translate(glm::mat4(1.0f), translateMat);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(locX, locY, locZ));
	//translate = trans * translate;
	//glm::mat4 orbit =
	//	glm::rotate(glm::mat4(1.0f), ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	//this->toWorld = translate * orbit;
	//this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, toWorld[3][1], 0.0f));
	//this->toWorld = glm::rotate(glm::mat4(1.0f), ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * this->toWorld;
	//glm::mat4 inverseTrans = glm::inverse(translate);
	this->center = trans * this->toWorld * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	box->setCenter(center);
	if ((check == 1) && (box->detectCollisionHeight() == 1)) 
	{
		trans = glm::mat4();
	}
	this->toWorld = trans * this->toWorld;
	//this->toWorld = glm::rotate(glm::mat4(1.0f), ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * this->toWorld;
	/**this->toWorld = translate * this->toWorld;
	translate = glm::translate(glm::mat4(), glm::vec3(locX, locY, locZ));
	this->toWorld = translate * this->toWorld;
	*/
}