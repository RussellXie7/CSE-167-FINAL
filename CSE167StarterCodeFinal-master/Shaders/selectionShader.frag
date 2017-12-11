#version 330 core

in vec3 fragPosition;

out vec4 finalColor;

uniform mat4 model;

uniform int code;

void main()
{
	finalColor = vec4(code/255.0,0,0, 0);
}




