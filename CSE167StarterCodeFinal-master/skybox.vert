#version 330 core

layout (location = 0) in vec3 vertex;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main() 
{
    gl_Position =  projection * view * vec4(vertex.x, vertex.y, vertex.z, 1.0f);  
    TexCoords = vertex;
}
