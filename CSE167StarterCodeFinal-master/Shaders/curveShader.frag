#version 330 core

in vec3 fragPosition;

out vec4 finalColor;

uniform mat4 model;
uniform int is_accel;

void main()
{
	if (is_accel == 1){
		finalColor = vec4(0,1,1, 1);
	}else{
		finalColor = vec4(0,0,0, 1);
	}
}




