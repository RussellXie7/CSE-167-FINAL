#version 330 core

const float PI = 3.1415927;
const float waveLength = 1.0f;
const float waveAmplitude = 0.2f;
const float specularReflectivity = 0.4f;
const float shineDamper = 4.0f;
const vec3 lightColor = vec3(1.0f, 1.3f, 1.5f);
const vec3 lightDir = vec3(12.5f, 42.9f, -21.5f);

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 neighbor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 camPos;
uniform float waterLevel;
uniform float waveTime;

out vec4 clipSpaceReal;
out vec4 clipSpaceGrid;

out vec3 camVec;
out vec3 faceNormal;

out vec3 specular;
out vec3 diffuse;

float genNoise(float x, float z, float val1, float val2) {
  float radiansX = ((mod(x+z*x*val1, waveLength)/waveLength) 
      + waveTime * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
  float radiansZ = ((mod(val2 * (z*x +x*z), waveLength)/waveLength) 
      + waveTime * 2.0 * mod(x , 2.0) ) * 2.0 * PI;
  return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 genWave(vec3 v) {
  float dx = genNoise(v.x, v.z, 0.2, 0.1);
  float dy = genNoise(v.x, v.z, 0.1, 0.3);
  float dz = genNoise(v.x, v.z, 0.15, 0.2);

  return v + vec3(dx, dy, dz);
}

vec3 calcNormal(vec3 v0, vec3 v1, vec3 v2) {
  vec3 dir0 = v1 - v0;
  vec3 dir1 = v2 - v0;
  return normalize(cross(dir0, dir1));
}

vec3 calcSpecular(vec3 toCamVector, vec3 toLightVector, vec3 normal){
  vec3 reflectedLightDirection = reflect(toLightVector, normal);
  float specularFactor = dot(reflectedLightDirection , toCamVector);
  specularFactor = max(specularFactor,0.0);
  specularFactor = pow(specularFactor, shineDamper);
  return specularFactor * specularReflectivity * lightColor;
}

vec3 calcDiffuse(vec3 toLightVector, vec3 normal) {
  float brightness = max(dot(toLightVector, normal), 0.0);
  return brightness * lightColor;
}

void main(void) {

  vec3 vertex0 = vec3(model * vec4(position.x, 0.0f, position.y, 1.0f));
  vertex0.y = waterLevel;
  vec3 vertex1 = vec3(model * vec4(neighbor.x, 0.0f, neighbor.y, 1.0f));
  vertex1.y = waterLevel;
  vec3 vertex2 = vec3(model * vec4(neighbor.z, 0.0f, neighbor.w, 1.0f));
  vertex2.y = waterLevel;

  clipSpaceGrid = projection * view * vec4(vertex0, 1.0f);

  vertex0 = genWave(vertex0);
  vertex1 = genWave(vertex1);
  vertex2 = genWave(vertex2);
  
  faceNormal = calcNormal(vertex0, vertex1, vertex2);
  if (dot(faceNormal, vec3(0, 1.0f, 0)) < 0) {
    faceNormal = -faceNormal;
  }

  clipSpaceReal = projection * view * vec4(vertex0, 1.0f);
  gl_Position = clipSpaceReal;

  camVec = normalize(camPos - vertex0);

  vec3 toLight = normalize(lightDir);
  specular = calcSpecular(camVec, toLight, faceNormal);
  diffuse = calcDiffuse(toLight, faceNormal);
}
