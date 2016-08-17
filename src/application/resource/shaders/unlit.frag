#version 410 core

in vec2 fUV;

out vec4 colour;

uniform sampler2D sTexture0;

void main()
{
	// Get our colour from the texture.
	colour = texture(sTexture0, fUV);
}