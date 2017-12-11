#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

in vec2 position;
in vec2 texcoord;

out vec2 vertTexCoord;

void main()
{
	vertTexCoord = texcoord;

    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = vec4(position,0.0,1.0);

}
