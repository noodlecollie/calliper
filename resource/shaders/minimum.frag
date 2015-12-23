#version 410 core
uniform vec3 inCol;
out vec4 color;

void main()
{
	color = vec4(inCol, 1);
}