#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragColor;

in vec3 lightDir0;
in vec3 lightDir1;

out vec4 color;

void main() {
  vec3 norm = normalize(fragNormal);
  vec3 result = vec3(0.0f, 0.0f, 0.0f);

  // directional light 
  vec3 lightDir0 = vec3(0.0f, 1.0f, 0.0f);

  // point light 
  vec3 lightDir1 = vec3(12.5f, 42.9f, -21.5f);

  // calc directional
  result += fragColor * 0.7f;
  result += fragColor * max(dot(norm, -lightDir0), 0.0f) * 0.01f;
  result += fragColor * max(dot(norm, -lightDir1), 0.0f) * 0.01f; 

  color = vec4(result * 0.8f, 1.0f);
}
