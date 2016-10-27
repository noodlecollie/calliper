#version 410 core

// Inputs from vertex shader
in vec4 fColour;
in vec3 fNormal;
in vec2 fTexCoord;

// Outputs
layout(location = 0) out vec4 color;

// Uniforms
uniform sampler2D tex;

// TODO: Put this in uniforms.
const vec3 LIGHT_DIR = vec3(0.577, 0.577, 0.577);

const float BASE_COL_MULTIPLIER = 0.5;

void main()
{
	// Get how closely the normal coincides with the light.
	float dotProductWithLight = dot(fNormal, LIGHT_DIR);

	// Remap so the value is between 0 and 1.
	dotProductWithLight += 1.0;
	dotProductWithLight /= 2.0;

	// Generate a multiplier for the colour, between BASE_COL_MULTIPLIER and 1.
	float mult = BASE_COL_MULTIPLIER + ((1-BASE_COL_MULTIPLIER) * dotProductWithLight);

	// Do a simple blend.
    color = texture(tex, fTexCoord) * fColour * mult;
}
