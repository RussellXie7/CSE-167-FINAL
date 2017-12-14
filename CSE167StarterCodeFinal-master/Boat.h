#ifndef BOAT_H
#define BOAT_H

#include "LowPolyOBJ.h"
#include "Terrain.h"

#define MAX_SPEED 0.5f
#define TURN 0.005f

struct Boat {
  LowPolyOBJ *obj;
  glm::mat4 translate, orbit, wave, scale;
  Terrain * terrain;
  float waveClock;
  float toggleClock;
  int viewMode;
  glm::vec3 dir, pos, camPos, camLookAt;
  float speed;
  float angle;
  
  Boat(char *, std::function<glm::vec3(glm::vec3, float)>, Terrain *);
  void forward();
  void turn(int);
  void toggleView();
  int freeCamMove();
  void update();
  void draw(GLuint);
};

#endif
