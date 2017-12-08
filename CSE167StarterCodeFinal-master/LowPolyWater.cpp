#include "LowPolyWater.h"

LowPolyWaterPt::LowPolyWaterPt(glm::vec2 pt) {
  for (int i = 0; i < 6; i++) {
    pts[i].vertex = pt;
    pts[i].used = 0;
  }
}

int LowPolyWaterPt::setNeighbor(glm::vec4 neighbor) {
  int i = 0;
  for (; i < 6; i++) {
    if (!pts[i].used)
      break;
  }

  if (i == 6)
    return -1;

  pts[i].neighbors = neighbor;

  return i;
}

glm::vec2 LowPolyWaterPt::getCoord() {
  return pts[0].vertex;
}

LowPolyWater::LowPolyWater(int radius, float waterLevel, int priority) {
  this->radius = radius;
  this->waterLevel = waterLevel;
  this->priority = priority;
  this->toWorld = glm::mat4(1.0f);

  this->refraction = CameraTexture(priority);
  this->refraction = CameraTexture(priority);

  generateWaterVec();
  generateFaces();
  bindVAOVBOEBO();
}

LowPolyWater::generateWaterVec() {
  for(int j = 0; j < 2 * radius; j++) {
    for (int i = 0; i < 2 * radius; i++) {
      float x = float(i - radius + 0.5f);
      float y = float(i - radius + 0.5f);
      vertices.push_back(LowPolyWaterPt(glm::vec2(x, y)));
    }
  }
}

LowPolyWater::generateFaces() {
  for (int j = 0; j < 2 * radius - 1; j++) {

    int iter = 0;
    int type = 0;

    // iterate thru rowStart to rowEnd
    int hasNext = 1;
    while (iter < 2 * radius) {

      if (!type) {
        idx0 = iter + 2 * radius * j;
        idx1 = iter + 1 + 2 * radius * j;
        idx2 = iter + 2 * radius * (j + 1);
      } else {
        idx0 = iter - 1 + 2 * radius * (j + 1);
        idx1 = iter + 2 * radius * (j + 1);
        idx2 = iter + 2 * radius * j;
      } 

      glm::vec2 pos0 = vertices[idx0].getCoord();
      glm::vec2 pos1 = vertices[idx1].getCoord();
      glm::vec2 pos2 = vertices[idx2].getCoord();
      
      // 2 1, 0 2, 1 0
      int cpIdx0 = vertices[idx0].setNeighbor(
          glm::vec4(pos2.x, pos2.y, pos1.x, pos1.y));
      int cpIdx1 = vertices[idx1].setNeighbor(
          glm::vec4(pos0.x, pos0.y, pos2.x, pos2.y));
      int cpIdx2 = vertices[idx2].setNeighbor(
          glm::vec4(pos1.x, pos1.y, pos0.x, pos0.y));

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

void LowPolyWater::bindVAOVBOEBO() {    
  glGenVertexArray(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LowPolyWaterPt),
      vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint),
      faces.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(WaterPt), 
     (GLvoid *) offsetof(WaterPt, vertex));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(WaterPt), 
     (GLvoid *) offsetof(WaterPt, neighbors));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void LowPolyWater::prepTexture() {
  // refraction takes photo of things below water level
  refraction.takeSnapShot(Window::cam_pos, Window::cam_look_at, 
      Window::cam_up, glm::vec4(0.0f, 1.0f, 0.0f, waterLevel));

  // reflection takes photo of things above water level
  glm::vec3 reflectCamPos = Window::cam_pos;
  glm::vec3 reflectCamLookAt = Window::cam_look_at;

  reflectCamPos.y -= 2.0f * (reflectCamPos.y - waterLevel);
  reflectCamLookAt.y -= 2.0f * (reflectCamLookAt.y - waterLevel);

  reflection.takeSnapShot(reflectCamPos, reflectCamLookAt,
      -Window::cam_up, glm::vec4(0.0f, -1.0f, 0.0f, waterLevel));
}

void LowPolyWater::draw(GLuint shader, int priority) {
  // only render when priority is lower than my priority
  if (priority >= this->priority)
    return;

  glUseProgram(shader);

  glUniformMatrix4fv(glGetUniformLocation(shader, "projection"),
    1, GL_FALSE, &Window::P[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader, "view"),
    1, GL_FALSE, &Window::V[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader, "model"),
    1, GL_FALSE, &toWorld[0][0]);

  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, refraction.texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, reflection.texture);
  
  glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);
}

