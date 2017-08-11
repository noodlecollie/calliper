#version 410 core

// Inputs from vertex shader
in vec4 fColour;

// Outputs
layout(location = 0) out vec4 color;

void main()
{
    color = fColour;
}
