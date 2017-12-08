#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 neighbor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float waterLevel;

out vec4 clipSpace;

void main(void) {
  clipSpace = projection * view * model 
      * vec4(position.x, waterLevel, position.y, 1.0f);
  gl_Position = clipSpace;
}
