#version 410 core

in vec4 fColor;

out vec4 color;

void main()
{
	// Just pass the colour through.
	color = fColor;
}