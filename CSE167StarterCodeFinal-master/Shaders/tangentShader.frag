#version 330 core

in vec3 fragPosition;

out vec4 finalColor;

uniform mat4 model;
uniform mat4 is_accel;


void main()
{
	finalColor = vec4(1,1,0, 1);
}




