#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;
uniform mat4 view;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragColor;

void main() {
  gl_Position = projection * modelview * vec4(position, 1.0f);
  fragPos  = vec3(model * vec4(position, 1.0f));
  // fragNormal = mat3(transpose(inverse(modelview))) */ normal;
  fragNormal = normal;
  fragColor = color;
}
