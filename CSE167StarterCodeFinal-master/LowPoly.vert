#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;
uniform mat4 view;
uniform vec4 clipPlane;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragColor;

void main() {
  vec4 worldPos = model * vec4(position, 1.0f);

  fragPos  = vec3(worldPos);
  fragNormal = normal;
  fragColor = color;

  gl_Position = projection * modelview * vec4(position, 1.0f);
  gl_ClipDistance[0] = dot(worldPos, clipPlane);
}
