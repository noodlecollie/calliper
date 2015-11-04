#version 330 core
uniform vec3 vec_Color;	// Colour for all fragments
out vec3 vec_ColorOut;
void main()
{
	vec_ColorOut = vec_Color;
};