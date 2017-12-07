#include "LowPolyOBJ.h"

LowPolyOBJ::LowPolyOBJ(int inOrOut, 
                  char * filename,
                  std::function<glm::vec3(glm::vec3, float)> colorFunc) {
  
  this->inOrOut = inOrOut;
  this->colorFunc = colorFunc;  
  this->toWorld = glm::mat4(1.0f);

  parse(filename);
  bindVAOVBOEBO();
}

void LowPolyOBJ::parse(char * filename) {

  std::vector<int> faces_t;
  std::vector<glm::vec3> vertices_t;

  float x,y,z;      		// point coordinate
  float r,g,b;      		// point color
  float x_min, x_max, y_min, y_max, z_min, z_max;

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
      int match = sscanf(buf, "f %d//%d %d//%d %d//%d\n", 
          &v1, &n1, &v2, &n2, &v3, &n3);
      
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

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

