#include "Terrain.h"

Terrain::Terrain(int radius, int inOrOut, 
                  std::function<glm::vec3(float, float, float)> upFunc,
                  std::function<glm::vec3(float, float, float)> downFunc,
                  std::function<glm::vec3(glm::vec3, float)> colorFunc) {
  
  this->radius = radius;
  this->inOrOut = inOrOut;
  this->upFunc = upFunc;
  this->downFunc = downFunc;
  this->colorFunc = colorFunc;  
  this->toWorld = glm::mat4(1.0f);

  generateTerrainVec();
  generateFaces();
  bindVAOVBOEBO();
}

void Terrain::generateTerrainVec() {
  // fill the points for upside
  LowPolyPt::setColorInterpolateFunc(colorFunc);
  LowPolyPt::setRadiusScale(radius);

  for (int k = 0; k < 2; k++) {
    // iterate twice first to generate up side, then down side
    std::function<glm::vec3(float, float, float)> yGen;
    if (k == 0) 
      yGen = upFunc;
    else
      yGen = downFunc;

    for (int j = 0; j < 2 * radius; j++) {
      for (int i = 0; i < 2 * radius; i++) {
        float x = float(i - radius + 0.5);
        float z = float(j - radius + 0.5);
        vertices.push_back(LowPolyPt(yGen(x, z, radius - 0.5f)));
      }
    }
  }

  // copy points at edge;
  for (int i = 0; i < 2 * radius; i++) {
    vertices[i] = vertices[8 * radius * radius - 1 - i];
    vertices[4 * radius * radius - 1 - i] = vertices[i + 4 * radius * radius];
    vertices[(2 * radius - 1 - i) * 2 * radius] = vertices[(i + 1) * 2 * radius - 1 + 4 * radius *radius];
    vertices[(i + 1) * 2 * radius - 1] = vertices[(2 * radius - 1 - i) * 2 * radius + 4 * radius * radius];
  }
}

/**
 * There 4 type of triangle
 *  ----
 *  | /   | type 0, if row start == next row start, start with this type
 *  |/   
 *    /|
 *   / |  | type 1, if row start <= next row start - 1, start with this type
 *  ----
 *  
 *  during iteration, draw type 1, 2 alternatively
 *
 *  ----
 *   \ |  | type 2, if row start >= next row + 1, start with this type then swich to type 1
 *    \|
 *  |\
 *  | \   | type 3, if row end <= next row end + 1, draw this trangle at end 
 *  ----
 */
void Terrain::generateFaces() {
  for (int k = 0; k < 2; k++) {
    // iterate rows
    for (int j = 0; j < 2 * radius - 1; j++) {

      int iter = 0;
      int type = 0;

      // iterate thru rowStart to rowEnd
      int hasNext = 1;
      while (iter < 2 * radius) {

        int idx0 = k? 4 * radius * radius: 0;
        int idx1 = k? 4 * radius * radius: 0;
        int idx2 = k? 4 * radius * radius: 0;

        if (!type) {
          idx0 += iter + 2 * radius * j;
          idx1 += iter + 1 + 2 * radius * j;
          idx2 += iter + 2 * radius * (j + 1);
        } else {
          idx0 += iter - 1 + 2 * radius * (j + 1);
          idx1 += iter + 2 * radius * j;
          idx2 += iter + 2 * radius * (j + 1);
        } 

        if (k && iter == 2 * radius - 1) {
          int total = vertices.size();
          idx0 = idx0 % total;
          idx1 = idx1 % total;
          idx2 = idx2 % total;
        }

        glm::vec3 pos0 = vertices[idx0].getCoord();
        glm::vec3 pos1 = vertices[idx1].getCoord();
        glm::vec3 pos2 = vertices[idx2].getCoord();

        glm::vec3 dir0 = pos0 - pos1;
        glm::vec3 dir1 = pos0 - pos2;
        glm::vec3 norm = glm::normalize(glm::cross(dir0, dir1));

        // make sure norm point to direction in or out specified
        if (glm::dot(norm, pos0) * inOrOut < 0)
          norm = -norm;
        
        // vertices have six copies of pt, return which copy get norm set
        int cpIdx0 = vertices[idx0].setNorm(norm);
        int cpIdx1 = vertices[idx1].setNorm(norm);
        int cpIdx2 = vertices[idx2].setNorm(norm);

        // copy index should be greater than 0
        if (cpIdx0 < 0 || cpIdx1 < 0 || cpIdx2 < 0) {
          std::cerr << "[ERROR] No Norm set!!!\n";
          std::cerr << iter << ", " << j << "\n";
          std::cerr << idx0 << ", " << idx1 << ", " << idx2 << "\n";
          std::cerr << cpIdx0 << ", " << cpIdx1 << ", " << cpIdx2 << "\n";
          std::cerr << faces.size() << "\n";
          exit(1);
        }

        faces.push_back(6 * idx0 + cpIdx0);
        faces.push_back(6 * idx1 + cpIdx1);
        faces.push_back(6 * idx2 + cpIdx2);

        // check how to increment iter
        if (!type) {
          type = 1;
          iter++;
        } else {
          if (iter == 2 * radius - 1) 
            break;
          type = 0;
        }
      }
    }
  }
}

void Terrain::bindVAOVBOEBO() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LowPolyPt),
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

void Terrain::draw(GLuint shader) {
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
