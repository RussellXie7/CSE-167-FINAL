#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragColor;

in vec3 lightDir0;
in vec3 lightDir1;
uniform int toon;
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
  vec3 diff1 = fragColor * max(abs(dot(norm, -lightDir0)), 0.0f) * 0.01f;
  //result += fragColor * max(abs(dot(norm, -lightDir0)), 0.0f) * 0.01f;
  result += diff1;

  vec3 diff2 = fragColor * max(abs(dot(norm, -lightDir1)), 0.0f) * 0.01f; 
  result += diff2;
  color = vec4(result * 0.8f, 1.0f);
//  // Toon shading
if(toon == 1) {
	vec3 viewPos = vec3(0.0f,1.0f,1.0f);
	float intensity;
	intensity = dot(normalize(viewPos - fragPos),normalize(fragNormal));
	float toon_shade = 1.0f;
	if (intensity > 0.85)
		color = vec4(color.x,0.5 * color.y,0.5 * color.z,1.0);
	else if (intensity > 0.45)
		color = vec4(0.6 * color.x,0.3 * color.y,0.3* color.z,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4* color.x,0.2* color.y,0.2* color.z,1.0);
	else
		color = vec4(0.2* color.x,0.1* color.y,0.1* color.z,1.0);
}
if(toon == 2) {
	vec3 viewPos = vec3(0.0f,1.0f,1.0f);
	float intensity;
	intensity = dot(normalize(viewPos - fragPos),normalize(fragNormal));
	float toon_shade = 1.0f;
	if (intensity > 0.85)
		color = vec4(0.9 * color.x, 0.8 * color.y,0.5 * color.z,1.0);
	else if (intensity > 0.45)
		color = vec4(0.65 * color.x,0.55 * color.y,0.3* color.z,1.0);
	else if (intensity > 0.25)
		color = vec4(0.45* color.x,0.35* color.y,0.2* color.z,1.0);
	else
		color = vec4(0.25* color.x,0.2* color.y,0.1* color.z,1.0);
}
//// end of toon shading
}
