#version 400 core

in vec4 clipSpace;

out vec4 color;

uniform sampler2D reflectTex;
uniform sampler2D refractTex;

void main(void) {
  vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0f + 0.5;

  vec4 refractColor = texture(refractTex, ndc);
  vec4 reflectColor = texture(reflectTex, vec2(ndc.x, 1.0f-ndc.y));

  color = mix(refractColor, reflectColor, 0.5f);
  color = mix(color, vec4(0.5f, 0.7f, 0.8f, 1.0f), 0.5f);
}
