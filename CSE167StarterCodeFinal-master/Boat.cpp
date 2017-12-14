#include "Boat.h"

Boat::Boat(char * objFile, std::function<glm::vec3(glm::vec3, float)> colorFunc, Terrain * terrain) {

  this->obj = new LowPolyOBJ(1, objFile, colorFunc);
  this->translate = glm::mat4(1.0f);
  this->orbit = glm::mat4(1.0f);
  this->wave = glm::mat4(1.0f);
  this->scale = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));

  this->terrain = terrain;
  this->viewMode = 0;
  this->toggleClock = 0.0f;
  this->waveClock = 0.0f;

  this->dir = glm::vec3(0.0f, 0.0f, 1.0f);
  this->speed = 0.0f;
  this->angle = 0.0f;
  this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Boat::forward() {
  if (!viewMode || toggleClock != 0.0f) 
    return;
  speed += 0.02f;
}

void Boat::turn(int LoR) {
  if (!viewMode || toggleClock != 0.0f) 
    return;
  angle += 0.01f * LoR;

  orbit = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  dir = glm::vec3(orbit * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void Boat::toggleView() {
  if (toggleClock != 0.0f)
    return;

  toggleClock = 1.0f;
  viewMode = viewMode? 0: 1;
  if (viewMode) {
    camPos = Window::cam_pos;
    camLookAt = Window::cam_look_at;
  }
}

void Boat::update() {
  // update cam pos
  if (toggleClock != 0.0f) {
    if (viewMode) {
      Window::cam_pos = camPos * toggleClock + (pos + glm::vec3(0.0f, 4.5f, 0.0f)) * (1 - toggleClock);
      Window::cam_look_at = camLookAt * toggleClock + (pos + dir) * (1 - toggleClock);
    } else {
      Window::cam_pos = (pos + glm::vec3(0.0f, 4.5f, 0.0f)) * toggleClock + camPos * (1 - toggleClock);
      Window::cam_look_at = camLookAt * (1 - toggleClock) + (camPos + dir) * toggleClock;
    }

    Window::V = glm::lookAt(Window::cam_pos, Window::cam_look_at, Window::cam_up);
    toggleClock -= 0.02f;
    if (toggleClock <= 0.0f) toggleClock = 0.0f;
  }
  
  // update orit translate, wave
  pos += speed * dir;

  translate = glm::translate(glm::mat4(1.0f), pos);

  // get water height
  glm::vec3 head = glm::vec3(1.0f, 0.0f, 0.0f);
  glm::vec3 side = glm::vec3(0.0f, 0.0f, 1.0f);

  int rad = terrain->radius;
  float headX = (head.x + head.z * head.x * 0.1f + waveClock * fmod(head.x * 0.8f + head.z, 1.5f)) * 2.0f * 3.1416f;
  float headZ = (0.4f * head.x * head.z + waveClock * 2.0f * fmod(head.x, 2.0f)) * 2.0f * 3.1416f;
  float sideX = (side.x + side.z * side.x * 0.1f + waveClock * fmod(side.x * 0.8f + side.z, 1.5f)) * 2.0f * 3.1416f;
  float sideZ = (0.4f * side.x * side.z + waveClock * 2.0f * fmod(side.x, 2.0f)) * 2.0f * 3.1416f;
  int selfX = int(Window::cam_pos.x + dir.x) + rad;
  int selfZ = int(Window::cam_pos.z + dir.z) + rad;

  float headAngle = atan(0.1f * (sin(headX) + cos(headZ)));
  float sideAngle = atan(0.1f * (sin(sideX) + cos(sideZ)));
  
  wave = glm::rotate(glm::mat4(1.0f), headAngle, glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)))
      * glm::rotate(glm::mat4(1.0f), sideAngle, dir);

  // check collision
  if (viewMode && toggleClock == 0.0f) {
    float selfY = terrain->vertices[selfZ * 2 * rad + selfX].getCoord().y;
    if (selfY > -2.0f) {
      glm::vec3 xxx =Window::cam_pos;
      xxx.y = 0.0f;
      pos += 2 * speed * glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - xxx);
      angle += 3.1416f /2.0f;
      orbit = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
      dir = glm::vec3(orbit * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    }
  }

  speed -= 0.001f;
  if (speed < 0.0f)
    speed = 0.0f;

  if (viewMode && toggleClock == 0.0f) {
    Window::cam_pos = pos + glm::vec3(0.0f, 2.5f, 0.0f) - glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * 4.0f - 2 * dir;
    Window::cam_look_at = Window::cam_pos + dir;
    Window::V = glm::lookAt(Window::cam_pos, Window::cam_look_at, Window::cam_up);
  }

  waveClock += 0.002f;
}

int Boat::freeCamMove() {
  return !viewMode && toggleClock == 0.0f;
}

void Boat::draw(GLuint shader) {
  obj->toWorld = translate * wave * scale * orbit ;
  glm::vec3 worldPos = glm::vec3(obj->toWorld * glm::vec4(pos, 1.0f));
  obj->draw(shader, 0);
  update();
}
