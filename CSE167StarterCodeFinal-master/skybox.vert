#version 330 core

layout (location = 0) in vec3 vertex;

uniform mat4 view;
uniform mat4 projection;
//uniform vec4 clipPlane;

out vec3 TexCoords;

void main() 
{
    gl_Position =  projection * view * vec4(vertex, 1.0f);  
 //   gl_ClipDistance[0] = dot(vec4(vertex, 1.0f), clipPlane);
    TexCoords = vertex;
}
