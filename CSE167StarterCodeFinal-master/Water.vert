#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 neighbor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float waterLevel;
uniform vec3 camPos;

out vec4 clipSpace;
out vec3 camVec;
out vec3 faceNormal;

void main(void) {

  clipSpace = projection * view * model 
      * vec4(position.x, waterLevel, position.y, 1.0f);
  
  faceNormal = vec3(0.0, 1.0, 0.0);

  vec3 worldPos = vec3(model * vec4(position.x, 0.0f, position.y, 1.0f));
  worldPos.y = waterLevel;
  camVec = normalize(camPos - worldPos);

  gl_Position = clipSpace;
}
