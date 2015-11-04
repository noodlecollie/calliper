#version 330 core
layout(location = 0) in vec3 vec_Position;	// Model space
uniform mat4 mat_MVP;
void main()
{
	gl_Position = mat_MVP * vec4(vec_Position,1);
}