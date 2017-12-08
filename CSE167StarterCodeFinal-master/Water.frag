#version 400 core

in vec4 clipSpace;
in vec3 camVec;
in vec3 faceNormal;

out vec4 color;

uniform sampler2D reflectTex;
uniform sampler2D refractTex;

const float fresnelK = 0.8f;

float calcFresnel() {
  float refractFact = dot(camVec, faceNormal);
  refractFact = pow(refractFact, fresnelK);
  return clamp(refractFact, 0.0, 1.0);
}

void main(void) {
  vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0f + 0.5;

  vec4 refractColor = texture(refractTex, ndc);
  vec4 reflectColor = texture(reflectTex, vec2(ndc.x, 1.0f-ndc.y));

  color = mix(reflectColor, refractColor, calcFresnel());
}
