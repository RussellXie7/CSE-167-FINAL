#include "TextureOBJ.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

vertTexPair::vertTexPair(glm::vec3 vertex, glm::vec2 tex) {
  this->vertex = vertex;
  this->tex = tex;
}


TextureOBJ::TextureOBJ(char * filename, char * texfile) {
  
  this->toWorld = glm::mat4(1.0f);

  parse(filename);
  loadTex(texfile);
  bindVAOVBOEBO();
}

void TextureOBJ::parse(char * filename) {

  std::vector<glm::vec3> vertices_t;
  std::vector<glm::vec2> tex_t;

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
      GLuint n0, n1, n2, n3, v0, v1, v2, v3, t0, t1, t2, t3;
      int match = sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
          &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);

      vertTexPair p0(vertices_t[v0], tex_t[t0]);
      vertTexPair p1(vertices_t[v1], tex_t[t1]);
      vertTexPair p2(vertices_t[v2], tex_t[t2]);

      if (match == 9) {
        int idx = vertices.size();
        vertices.push_back(p0);
        vertices.push_back(p1);
        vertices.push_back(p2);
        faces.push_back(idx);
        faces.push_back(idx + 1);
        faces.push_back(idx + 2);
        
      } else if (match == 12) {
        vertTexPair p3(vertices_t[v3], tex_t[t3]);
        int idx = vertices.size();
        vertices.push_back(p0);
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
        faces.push_back(idx);
        faces.push_back(idx + 1);
        faces.push_back(idx + 2);
        faces.push_back(idx);
        faces.push_back(idx + 2);
        faces.push_back(idx + 3);
      }
  
    // parse vertex 
    } else if (buf[0] == 'v' && buf[1] == ' ') {
      int match = sscanf(buf, "v %f %f %f\n", &x, &y, &z); 
      
			if (match != 3) {
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

    } else if (buf[0] == 'v' && buf[1] == 't') {
      int match = sscanf(buf, "vt %f %f %f\n", &x, &y, &z); 
      
			if (match != 3) {
				std::cerr << buf << std::endl;
				std::cerr << x << " " << y << " " << z << std::endl;
        std::cerr << "Failed to parse line " << lineNum << std::endl;
    		lineNum++;
        continue;
      } 

      tex_t.push_back(glm::vec2(x, y)); 
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

	for (unsigned int i = 0; i < vertices.size(); ++i) {
    vertices[i].vertex.x = (vertices[i].vertex.x - transX) * scale;
    vertices[i].vertex.y = (vertices[i].vertex.y - transY) * scale;
    vertices[i].vertex.z = (vertices[i].vertex.z - transZ) * scale;
  }
}

void TextureOBJ::loadTex(char *file) {
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureOBJ::bindVAOVBOEBO() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertTexPair),
      vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint),
      faces.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertTexPair), 
     (GLvoid *) offsetof(vertTexPair, vertex));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertTexPair), 
     (GLvoid *) offsetof(vertTexPair, tex));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextureOBJ::draw(GLuint shader) {
  glm::mat4 modelView = Window::V * toWorld;

  glUseProgram(shader);
  glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 
      1, GL_FALSE, &Window::P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 
      1, GL_FALSE, &Window::V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader, "model"),
      1, GL_FALSE, &toWorld[0][0]);

  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}


