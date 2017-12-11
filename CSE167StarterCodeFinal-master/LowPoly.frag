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


 // // toon shading
	//float toon_shade_effect = 1.0;
	//float cosine = max(dot(norm, normalize(lightDir0)), 0.0);

	//if(cosine > 0.98)	toon_shade_effect = 1.0;
	//else if(cosine > 0.95)	toon_shade_effect = 0.9;
	//else if(cosine > 0.5)	toon_shade_effect = 0.7;
	//else if(cosine > 0.05)	toon_shade_effect = 0.35;
	//else	toon_shade_effect = 1.0;

	//float toon_shade_effect2 = 1.0;
	//float cosine2 = max(dot(norm, normalize(lightDir1)), 0.0);

	//if(cosine2 > 0.98)	toon_shade_effect2 = 1.0;
	//else if(cosine2 > 0.95)	toon_shade_effect2 = 0.9;
	//else if(cosine2 > 0.5)	toon_shade_effect2 = 0.7;
	//else if(cosine2 > 0.05)	toon_shade_effect2 = 0.35;
	//else	toon_shade_effect2 = 1.0;

	//result = (toon_shade_effect + toon_shade_effect2) * result;
 // // end of toon shading

  color = vec4(result * 0.8f, 1.0f);

//  // Toon shading
//	vec3 viewPos = vec3(0.0f,1.0f,1.0f);
//	float edge = dot(normalize(viewPos - fragPos), fragNormal);
//	edge = max(0, -edge);

//	if (edge < 0.0001)
//	{
//		color = 0.0 * color;
//	}
//// end of toon shading
}
