#version 410 core

// Inputs from vertex shader
in vec4 fColour;
in vec2 fTexCoord;

// Outputs
layout(location = 0) out vec4 color;

// Uniforms
uniform sampler2D tex;


void main()
{
	// Do a simple blend.
        color = texture(tex, fTexCoord) * fColour;
}
