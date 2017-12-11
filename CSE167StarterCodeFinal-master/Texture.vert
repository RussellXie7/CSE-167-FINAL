#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 clipPlane;

out vec2 texCoord;

void main() {
  vec4 worldPos = model * vec4(position, 1.0f);
  
  texCoord = tex;

  gl_Position = projection * view * model * vec4(position, 1.0f);
  gl_ClipDistance[0] = dot(worldPos, clipPlane);
}

