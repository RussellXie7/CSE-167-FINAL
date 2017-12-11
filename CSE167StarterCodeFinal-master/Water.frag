#version 330 core

in vec4 clipSpaceReal;
in vec4 clipSpaceGrid;

in vec3 camVec;
in vec3 faceNormal;

in vec3 specular;
in vec3 diffuse;

out vec4 color;

uniform sampler2D reflectTex;
uniform sampler2D refractTex;

const float fresnelK = 0.8f;

float calcFresnel() {
  float refractFact = dot(camVec, faceNormal);
  refractFact = pow(refractFact, fresnelK);
  return clamp(refractFact, 0.0, 1.0);
}

vec2 to_ndc(vec4 v) {
  return v.xy / v.w / 2.0f + 0.5f;
}

void main(void) {
  vec2 texReal = to_ndc(clipSpaceReal);
  vec2 texGrid = to_ndc(clipSpaceGrid);

  vec4 refractColor = texture(refractTex, texGrid);
  vec4 reflectColor = texture(reflectTex, vec2(texGrid.x, 1.0f-texGrid.y));

  color = mix(reflectColor, refractColor, calcFresnel());
  color = vec4(vec3(color) * diffuse + specular, 1.0f);
}
